#pragma once

template <typename T>
struct IsPOD
{
	enum
	{
		value = false
	};
	static void Create( T* pointer )
	{
		new ( pointer ) T();
	}
	static void Create( T* pointer, T const* oldData )
	{
		new ( pointer ) T( *oldData );
	}
	static void Create( T* pointer, T const* oldData, unsigned int const num )
	{
		for ( unsigned int i = 0; i < num; ++i )
		{
			new ( &pointer[i] ) T( oldData[i] );
		}
	}
	static void Create( T* pointer, unsigned int const num )
	{
		for ( unsigned int i = 0; i < num; ++i )
		{
			new ( &pointer[i] ) T();
		}
	}
	static void Create( T* pointer, unsigned int start, unsigned int const num )
	{
		for ( ; start < num; ++start )
		{
			new ( &pointer[start] ) T();
		}
	}
	static void Destroy(T* pointer)
	{
		pointer->~T();
	}

	static void Destroy(T* pointer, unsigned int const num )
	{
		for ( unsigned int i = 0; i < num; ++i )
		{
			pointer[i].~T();
		}
	}
	static void Destroy(T* pointer, unsigned int start, unsigned int const num)
	{
		for ( ; start < num; ++start )
		{
			pointer[start].~T();
		}
	}
};
template <typename T>
struct IsPOD<T*>
{
	enum
	{
		value = true
	};
	static void Create( T** pointer )
	{
	}
	static void Create( T** pointer, T* const* oldData )
	{
		memcpy( pointer, oldData, sizeof( T* ) );
	}
	static void Create( T** pointer, T* const* oldData, unsigned int const num )
	{
		memcpy( pointer, oldData, num * sizeof( T* ) );
	}
	static void Create( T** pointer, unsigned int start, unsigned int const num )
	{
	}
	static void Create( T** pointer, unsigned int const num )
	{
	}
	static void Destroy(T** pointer)
	{
	}
	static void Destroy(T** pointer, unsigned int const num )
	{
	}
	static void Destroy(T** pointer, unsigned int start, unsigned int const num)
	{
	}
};

#define POD_TYPE( T )													\
template<> struct IsPOD< T >											\
{																		\
enum { value = true };													\
static void Create( T* pointer ){}										\
static void Create( T* pointer, T const* oldData )						\
{																		\
	memcpy( pointer, oldData, sizeof( T ) );							\
}																		\
static void Create( T* pointer, T const* oldData, unsigned int const num )		\
{																		\
	memcpy( pointer, oldData, num * sizeof( T ) );						\
}																		\
static void Create( T* pointer, unsigned int const num ){}						\
static void Create( T* pointer, unsigned int start, unsigned int const num ){}			\
static void Destroy(T* pointer){}										\
static void Destroy(T* pointer, unsigned int const num ){}						\
static void Destroy(T* pointer, unsigned int start, unsigned int const num){}			\
};																		\

template <typename T >
class TArray
{
private:
	T* m_data;
	unsigned int m_size;
	unsigned int m_allocSize;

private:
	void Reallocate( unsigned int const size )
	{
		if ( size != m_allocSize )
		{
			T* oldData = m_data;
			m_data = nullptr;

			if ( 0 < size )
			{
				m_data = (T*)malloc( size * sizeof( T ) );
				unsigned int const copySize = size < m_size ? size : m_size;
				IsPOD< T >::Create( m_data, oldData, copySize );
			}

			IsPOD< T >::Destroy( oldData, m_size );
			free( oldData );

			m_allocSize = size;
			m_size = m_allocSize < m_size ? m_allocSize : m_size;
		}
	}

public:
	TArray()
		: m_data( nullptr )
		, m_allocSize( 0 )
		, m_size( 0 )
	{}

	TArray( unsigned int const allocSize )
		: m_data( nullptr )
		, m_allocSize( 0 )
		, m_size( 0 )
	{
		Reallocate( allocSize );
	}

	TArray( TArray<T> const& other )
	{
		m_allocSize = other.m_allocSize;
		m_size = other.m_size;
		m_data = (T*)malloc( m_allocSize * sizeof( T ) );

		IsPOD< T >::Create( m_data, other.m_data, m_size );
	}

	~TArray()
	{
		IsPOD< T >::Destroy( m_data, m_size );
		free( m_data );
	}

	void Resize( unsigned int const size )
	{
		if ( m_size < size )
		{
			if ( m_allocSize < size )
			{
				Reallocate( size );
			}

			IsPOD< T >::Create( &m_data[ m_size ], m_size, size );
		}
		else if ( size < m_size )
		{
			IsPOD< T >::Destroy( &m_data[size], size, m_size );
		}
		m_size = size;
	}

	void Reserve( unsigned int const size )
	{
		if ( m_allocSize < size )
		{
			Reallocate( size );
		}
	}

	void Add( T const& object )
	{
		if ( m_allocSize <= m_size )
		{
			Reallocate( ( m_allocSize + 1 ) << 1 );
		}
		
		IsPOD< T >::Create( &m_data[ m_size ], &object );
		++m_size;
	}
	void Add()
	{
		if ( m_allocSize <= m_size )
		{
			Reallocate( ( m_allocSize + 1 ) << 1 );
		}
		else
		{
			IsPOD< T >::Create( &m_data[ m_size ] );
		}
		++m_size;
	}
	void EraseBack()
	{
		IsPOD< T >::Destroy( &m_data[ m_size - 1 ] );
		--m_size;
	}

	T PopBack()
	{
		T returnVal;

		IsPOD<T>::Create( &returnVal, &m_data[ m_size - 1 ] );
		EraseBack();

		return returnVal;
	}

	void Clear()
	{
		IsPOD< T >::Destroy( m_data, m_size );
		m_size = 0;
	}

	void Free()
	{
		Reallocate( 0 );
	}

	__forceinline T* begin() 
	{ 
		return &m_data[0]; 
	}
	__forceinline T const* begin() const 
	{ 
		return &m_data[0]; 
	}
	__forceinline T* end() 
	{ 
		return &m_data[m_size]; 
	}
	__forceinline T const* end() const 
	{ 
		return &m_data[m_size]; 
	}

	__forceinline T* Data()
	{
		return m_data;
	}
	__forceinline T const* Data() const
	{
		return m_data;
	}

	__forceinline T& GetAt( unsigned int i )
	{
		return m_data[ i ];
	}
	__forceinline T const& GetAt( unsigned int i ) const
	{
		return m_data[ i ];
	}

	__forceinline T& operator[]( unsigned int i )
	{
		return m_data[ i ];
	}
	__forceinline T const& operator[]( unsigned int i ) const
	{
		return m_data[ i ];
	}

	__forceinline unsigned int Size() const
	{
		return m_size;
	}

	void operator=( TArray<T> const& other )
	{
		IsPOD< T >::Destroy( m_data, m_size );

		if ( m_allocSize != other.m_allocSize )
		{
			free( m_data );
			m_data = (T*)malloc( other.m_allocSize * sizeof( T ) );
			m_allocSize = other.m_allocSize;
		}

		m_size = other.m_size;
		IsPOD< T >::Create( m_data, other.m_data, m_size );
	}
};

POD_TYPE(bool)
POD_TYPE(unsigned short)
POD_TYPE(unsigned int)
POD_TYPE(short)
POD_TYPE(int)
POD_TYPE(__int64)
POD_TYPE(unsigned char)
POD_TYPE(char)
POD_TYPE(float)
POD_TYPE(double)
POD_TYPE(Vec3)
