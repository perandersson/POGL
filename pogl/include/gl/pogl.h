#pragma once
#ifndef _POGL_H_
#define _POGL_H_

#include <exception>
#include <condition_variable>
#if defined(POGL_ENHANCED_INSTRUCTION_SET)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#include <cinttypes>

#ifndef OFFSET
#define OFFSET(x) ((char *)0 + x)
#endif

#ifndef BIT
#define BIT(x) 1 << x
#endif

#ifndef BIT_ISSET
#define BIT_ISSET(a, b) (( (a) & (b) ) != 0)
#endif

#ifndef BIT_UNSET
#define BIT_UNSET(a, b) (a &= ~b)
#endif

#ifndef BIT_SET
#define BIT_SET(a, b) (a |= (b))
#endif

#ifndef BIT_ALL
#define BIT_ALL ULONG_MAX
#endif

#ifndef BIT_NONE
#define BIT_NONE 0
#endif

#ifndef POGL_DECLARE_ALIGN
#ifdef WIN32
#define POGL_DECLARE_ALIGN(x) __declspec(align(x))
#else
#error Add align declaration here
#endif
#endif

//
// Classes
//

class IPOGLInterface;
class IPOGLResource;

class IPOGLDevice;
class IPOGLDeviceContext;
class IPOGLRenderState;

class IPOGLDeferredDeviceContext;

class IPOGLVertexBuffer;
class IPOGLIndexBuffer;

class IPOGLTexture;
class IPOGLTexture1D;
class IPOGLTexture2D;
class IPOGLTexture3D;
class IPOGLFramebuffer;

class IPOGLShaderProgram;
class IPOGLEffect;
class IPOGLUniform;

//
// typedefs
//

typedef int8_t POGL_INT8;
typedef uint8_t POGL_UINT8;
typedef int16_t POGL_INT16;
typedef uint16_t POGL_UINT16;
typedef int32_t POGL_INT32;
typedef uint32_t POGL_UINT32;
typedef int64_t POGL_INT64;
typedef uint64_t POGL_UINT64;
typedef void* POGL_HANDLE;
typedef float POGL_FLOAT;
typedef double POGL_DOUBLE;

#include <string>
#ifdef UNICODE
typedef wchar_t POGL_CHAR;
typedef std::wstring POGL_STRING;
#define POGL_TOSTRING(x) L##x
#define POGL_TOCHAR(x) L##x
#else
typedef char POGL_CHAR;
typedef std::string POGL_STRING;
#define POGL_TOSTRING(x) POGL_STRING(POGL_TOCHAR(x))
#define POGL_TOCHAR(x) x
#endif

//
// Enums
//

struct POGLDeviceInfoFlags
{
	enum Enum {
		//
		// This flag enables debug mode. Debug mode is differs depending on the graphics card you have. 
		// AMD, for example, enables us to attach CodeXL (a remote debugger) and change stuff in runtime
		//
		DEBUG_MODE = BIT(0)
	};
};

struct POGLVendor
{
	enum Enum {
		AMD = 0,
		NVIDIA,
		INTEL,
		SOFTWARE,
		UNKNOWN
	};
};

struct POGLTextureFormat
{
	enum Enum {
		R = 0,
		R16F,
		R32F,
		RGB,
		RGB8,
		RGB12,
		RGB16,
		RGB32,
		RGBA,
		RGBA8,
		RGBA12,
		RGBA16,
		RGB10_A2,
		RGBA16F,
		RGBA32F,
		BGR,
		BGRA,
		DEPTH24,
		DEPTH32F,
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8,

		/* Number of enums available */
		COUNT
	};
};

struct POGLPixelFormat
{
	enum Enum {
		R8G8B8A8 = 0,

		/* Number of enums available */
		COUNT
	};
};

struct POGLClearType
{
	enum Enum {
		COLOR = BIT(1),
		DEPTH = BIT(2),
		STENCIL = BIT(3)
	};

	/* Clear the color- and depth buffers */
	static const POGL_UINT32 COLOR_AND_DEPTH = COLOR | DEPTH;

	/* Clear the depth- and stencil buffers */
	static const POGL_UINT32 DEPTH_AND_STENCIL = DEPTH | STENCIL;

	/* Clear all buffers */
	static const POGL_UINT32 ALL = COLOR | DEPTH | STENCIL;
};

struct POGLShaderProgramType
{
	enum Enum {
		GEOMETRY_SHADER = 0,
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		/* Number of enums available */
		COUNT
	};
};

struct POGLVertexType
{
	enum Enum {
		BYTE = 0,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,
		FLOAT,
		DOUBLE,

		/* Number of enums available */
		COUNT
	};
};

struct POGLPrimitiveType
{
	enum Enum {
		POINT = 0,
		TRIANGLE,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		LINE,
		LINE_LOOP,

		/* Number of enums available */
		COUNT
	};
};

struct POGLBufferUsage
{
	enum Enum {
		STATIC = 0,
		DYNAMIC,

		/* Number of enums available */
		COUNT
	};
};

struct POGLDepthFunc
{
	enum Enum {
		NEVER = 0,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS,

		/* Number of enums available */
		COUNT
	};

	/* Default depth function used by the rendering engine */
	static const Enum DEFAULT = LEQUAL;
};

struct POGLTextureWrap
{
	enum Enum {
		CLAMP_TO_EDGE = 0,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		REPEAT,

		/* Number of enums available */
		COUNT
	};

	/* Default texture wrap used by the rendering engine */
	static const Enum DEFAULT = REPEAT;
};

struct POGLMinFilter
{
	enum Enum {
		NEAREST = 0,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR,

		/* Number of enums available */
		COUNT
	};

	/* Default min filter used by the rendering engine */
	static const Enum DEFAULT = LINEAR;
};

struct POGLMagFilter
{
	enum Enum {
		NEAREST = 0,
		LINEAR,

		/* Number of enums available */
		COUNT
	};

	/* Default mag filter used by the rendering engine */
	static const Enum DEFAULT = LINEAR;
};

/*!
	\brief
*/
struct POGLCompareFunc
{
	enum Enum {
		NEVER = 0,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS,

		/* Number of enums available */
		COUNT
	};

	/* Default compare function used by the rendering engine */
	static const Enum DEFAULT = LEQUAL;
};

/*!
	\brief
*/
struct POGLCompareMode
{
	enum Enum {
		COMPARE_R_TO_TEXTURE,
		NONE,

		/* Number of enums available */
		COUNT
	};

	/* Default compare mode used by the rendering engine */
	static const Enum DEFAULT = NONE;
};

/*!

*/
struct POGLPolygonMode
{
	enum Enum {
		POINT = 0,
		LINE,
		FILL,

		COUNT
	};

	/* Default polygon mode used by the rendering engine */
	static const Enum DEFAULT = FILL;
};

struct POGLColorMask
{
	enum Enum {
		RED = BIT(1),
		GREEN = BIT(2),
		BLUE = BIT(3),
		ALPHA = BIT(4)
	};

	/* Enable all color components */
	static const POGL_UINT8 ALL = RED | GREEN | BLUE | ALPHA;

	/* Disable all color components */
	static const POGL_UINT8 NONE = 0;
};

struct POGLSrcFactor
{
	enum Enum {
		ZERO = 0,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,

		COUNT
	};

	/* Default source factor used by the rendering engine */
	static const Enum DEFAULT = ONE;
};

struct POGLDstFactor
{
	enum Enum {
		ZERO = 0,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,

		COUNT
	};

	/* Default destination factor used by the rendering engine */
	static const Enum DEFAULT = ZERO;

};

/*!
	\brief Describes our intentions with the resource mapping
*/
struct POGLResourceMapType
{
	enum Enum {
		/* Open a reading stream */
		//READ = 0,

		/* Our intentions is to write data to the resource */
		WRITE = 0,

		/* Open the stream for reading and writing */
		//READ_AND_WRITE,
		COUNT
	};
};

/*!
	\brief Resource type enum
*/
struct POGLResourceType
{
	enum Enum {
		VERTEXBUFFER = 0,
		INDEXBUFFER,
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		SHADER,
		EFFECT,
		FRAMEBUFFER
	};
};

//
// Structs
//

#if !defined(POGL_ENHANCED_INSTRUCTION_SET)
typedef struct __POGL_VECTOR128
{
	union {
		POGL_UINT32 u[4];
		POGL_FLOAT  f[4];
	};
} __POGL_VECTOR128;

typedef struct __POGL_VECTOR64
{
	union {
		POGL_UINT32 u[2];
		POGL_FLOAT  f[2];
	};
} __POGL_VECTOR64;
#else
typedef __m128 __POGL_VECTOR128;
typedef __m64 __POGL_VECTOR64;
#endif

typedef struct POGL_SIZE
{
	union {
		struct {
			POGL_INT32 x;
			POGL_INT32 y;
		};
		struct {
			POGL_INT32 width;
			POGL_INT32 height;
		};
		POGL_INT32 size[2];
	};

	POGL_SIZE() {}
	POGL_SIZE(const POGL_INT32 _x, const POGL_INT32 _y) : x(_x), y(_y) {}
	POGL_SIZE(const POGL_SIZE& rhs) : x(rhs.x), y(rhs.y) {}

	POGL_SIZE& operator=(const POGL_SIZE& rhs);
} POGL_POINTI;

struct POGL_VECTOR2
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
		};
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
		};
		POGL_FLOAT vec[2];
	};
	POGL_VECTOR2() {}
	POGL_VECTOR2(const POGL_FLOAT _x, const POGL_FLOAT _y) : x(_x), y(_y) {}
	POGL_VECTOR2(const POGL_VECTOR2& rhs) : x(rhs.x), y(rhs.y) {}

	POGL_VECTOR2& operator=(const POGL_VECTOR2& rhs);
};

typedef struct POGL_VECTOR3
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
			POGL_FLOAT z;
		};
		struct {
			POGL_FLOAT r;
			POGL_FLOAT g;
			POGL_FLOAT b;
		};
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
			POGL_FLOAT r;
		};
		POGL_FLOAT vec[3];
	};

	POGL_VECTOR3() {}
	POGL_VECTOR3(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z) : x(_x), y(_y), z(_z) {}
	POGL_VECTOR3(const POGL_VECTOR3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	POGL_VECTOR3& operator=(const POGL_VECTOR3& rhs);
} POGL_COLOR3;

typedef struct POGL_VECTOR4
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
			POGL_FLOAT z;
			POGL_FLOAT w;
		};
		struct {
			POGL_FLOAT r;
			POGL_FLOAT g;
			POGL_FLOAT b;
			POGL_FLOAT a;
		};
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
			POGL_FLOAT r;
			POGL_FLOAT q;
		};
		POGL_FLOAT vec[4];
	};

	POGL_VECTOR4() {}
	POGL_VECTOR4(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z, const POGL_FLOAT _w) : x(_x), y(_y), z(_z), w(_w) {}
	POGL_VECTOR4(const POGL_VECTOR4& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

	POGL_VECTOR4& operator=(const POGL_VECTOR4& rhs);
} POGL_COLOR4;

struct POGL_RECT
{
	union {
		struct {
			POGL_INT32 x;
			POGL_INT32 y;
			POGL_INT32 width;
			POGL_INT32 height;
		};
		POGL_INT32 size[4];
	};

	POGL_RECT() {}
	POGL_RECT(const POGL_INT32 _x, const POGL_INT32 _y, const POGL_INT32 _width, const POGL_INT32 _height) : x(_x), y(_y), width(_width), height(_height) {}
	POGL_RECT(const POGL_RECT& rhs) : x(rhs.x), y(rhs.y), width(rhs.width), height(rhs.height) {}

	POGL_RECT& operator=(const POGL_RECT& rhs);
};

struct POGL_MAT4
{
	union {
		struct {
			POGL_FLOAT
			_11, _21, _31, _41,
			_12, _22, _32, _42,
			_13, _23, _33, _43,
			_14, _24, _34, _44;
		};
		struct {
			POGL_FLOAT m[4][4];
		};
		POGL_FLOAT vec[16];
	};
};

/*!
	\brief
*/
struct POGL_DEVICE_INFO
{
	/* The window the IPOGLDeviceContext should be associated with. If nullptr then the context will "draw" to the console instead */
	POGL_HANDLE windowHandle;

	/* How many bits are used for each color component */
	POGL_UINT8 colorBits;

	/* How many bits used for the depth buffer */
	POGL_UINT8 depthBits;

	/* The device pixel format */
	POGLPixelFormat::Enum pixelFormat;

	/* Extra flags for the device info, for example: DEBUG mode */
	POGL_UINT8 flags;
};

/*!
	\brief
*/
struct POGL_VERTEX_LAYOUT_FIELD
{
	/*!
		The size of the current buffer field type. Use {@code sizeof(Type)}, for example: {@code sizeof(POGL_VEC3F)}
	*/
	POGL_UINT8 fieldSize;

	/*!
		What datatype should the element be handled as in the shader. the POGL_VEC3F type has POGL_FLOAT values, which means
		that this field should be POGLVertexLayoutFieldType::FLOAT
	*/
	POGLVertexType::Enum type;

	/*!
		Tell the graphics driver if it should normalize the value before it being supplied to the shader program
	*/
	POGL_UINT8 normalize;
};

/* */
static const POGL_UINT32 MAX_VERTEX_LAYOUT_FIELD_SIZE = 8;

/*!
	\brief
*/
struct POGL_VERTEX_LAYOUT
{
	/* The vertex fields */
	POGL_VERTEX_LAYOUT_FIELD fields[MAX_VERTEX_LAYOUT_FIELD_SIZE];

	/* The size of each vertex. This value will also be used as an stride between each vertex (This is most likely sizeof(Type)). */
	POGL_UINT32 vertexSize;
};


/*!
	\brief Vertex containing a position
*/
struct POGL_POSITION_VERTEX
{
	POGL_VECTOR3 position;

	POGL_POSITION_VERTEX() {}
	POGL_POSITION_VERTEX(const POGL_VECTOR3& p) : position(p) {}
	POGL_POSITION_VERTEX(const POGL_POSITION_VERTEX& rhs) : position(rhs.position) {}

	POGL_POSITION_VERTEX& operator=(const POGL_POSITION_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_VERTEX_LAYOUT = {
	{
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_VERTEX)
};

/*!
	\brief Vertex containing a position and color
*/
struct POGL_POSITION_COLOR_VERTEX
{
	POGL_VECTOR3 position;
	POGL_COLOR4 color;

	POGL_POSITION_COLOR_VERTEX() {}
	POGL_POSITION_COLOR_VERTEX(const POGL_VECTOR3& p, const POGL_COLOR4& c) : position(p), color(c) {}
	POGL_POSITION_COLOR_VERTEX(const POGL_POSITION_COLOR_VERTEX& rhs) : position(rhs.position), color(rhs.color) {}

	POGL_POSITION_COLOR_VERTEX& operator=(const POGL_POSITION_COLOR_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_COLOR_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_COLOR_VERTEX_LAYOUT = {
	{ 
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		{ sizeof(POGL_COLOR4), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_COLOR_VERTEX)
};

/*!
	\brief Vertex containing a position and a texture coordinate
*/
struct POGL_POSITION_TEXCOORD_VERTEX
{
	POGL_VECTOR3 position;
	POGL_VECTOR2 texCoord;

	POGL_POSITION_TEXCOORD_VERTEX() {}
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_VECTOR3& p, const POGL_VECTOR2& t) : position(p), texCoord(t) {}
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_POSITION_TEXCOORD_VERTEX& rhs) : position(rhs.position), texCoord(rhs.texCoord) {}

	POGL_POSITION_TEXCOORD_VERTEX& operator=(const POGL_POSITION_TEXCOORD_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_TEXCOORD_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_TEXCOORD_VERTEX_LAYOUT = {
	{ 
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		{ 0 }, 
		{ sizeof(POGL_VECTOR2), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_TEXCOORD_VERTEX)
};

//
// Class Definitions
//

class IPOGLInterface
{
public:
	virtual ~IPOGLInterface() {}

	/*!
		\brief Increase the reference counter with 1
		
		Reference counters are used internally to prevent resources from being deleted when they are in use
	*/
	virtual void AddRef() = 0;

	/*!
		\brief Decrease the reference counter with 1

		Reference counters are used internally to prevent resources from being deleted when they are in use
	*/
	virtual void Release() = 0;
};

/*!
	\brief
*/
class IPOGLResource : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the resource type
	*/
	virtual POGLResourceType::Enum GetResourceType() const = 0;
};

/*!
	\brief 
*/
class IPOGLDevice : public IPOGLInterface
{
public:
	/*!
		\brief Creates a new device context

		You use the device context to draw the geometry onto the screen

		\return A device context
	*/
	virtual IPOGLDeviceContext* GetDeviceContext() = 0;
	
	/*!
		\brief Creates a new deferred device context

		A deferred device context is esentially a command queue implementation for the IPOGLDeviceContext interface. You invoke it as you normally would and
		then supply it to be rendered in the main thread when it's done

		\return A device context
	*/
	virtual IPOGLDeferredDeviceContext* CreateDeferredDeviceContext() = 0;

	/*!
		\brief Swap buffers
	*/
	virtual void EndFrame() = 0;

	/*!
		\brief Retrieves the device info
	*/
	virtual const POGL_DEVICE_INFO* GetDeviceInfo() const = 0;

	/*!
		\brief Retrieves the vendor for the current computer
	*/
	virtual POGLVendor::Enum GetVendor() const = 0;
};

/*!
	\brief 
*/
class IPOGLDeviceContext : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the device
	*/
	virtual IPOGLDevice* GetDevice() = 0;
	
	/*!
		\brief Creates a shader program based on the supplied file

		\param path
				The path to the file where the shader source code is located
		\param type
				The shader type
		\throws POGLResourceException
				Exception thrown if the shader source code is not found or is invalid
		\return A shader program
	*/
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type) = 0;

	/*!
		\brief Creates a shader program based on the memory buffer

		\param memory
				String containing the shader source code
		\param size
				The size of the memory
		\param type
				The shader type
		\throws POGLResourceException
				Exception thrown if the shader source code is invalid
		\return A shader program
	*/
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type) = 0;

	/*!
		\brief Creates an effect based on the supplied programs

		\param programs
				The programs we want to link when creating the effect. The array must end with a nullptr or 0. 
				{@code
					IPOGLShaderProgram** programs = {shader1, shader2, nullptr};
					context->CreateEffectFromPrograms(programs);
				}
		\throwd POGLResourceException 
				Exception is thrown if the exception failed to be loaded by some reason.
		\return An effect instance
	*/
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture1D* CreateTexture1D() = 0;

	/*!
		\brief Creates a 2D texture

		\param size
				Texture geometry size (width and height)
		\param format
				Texture format
		\param bytes
				Bytes containing the texture data.
		\return
	*/
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture3D* CreateTexture3D() = 0;

	/*!
		\brief Creates a framebuffer that renders to the supplied textures

		\param textures
				The textures we want to use for the framebuffer. The array must end with a nullptr or 0. 
				{@code
					IPOGLTexture** textures = {texture1, texture2, nullptr};
					context->CreateFramebuffer(textures);
				}
		\return A framebuffer instance. You can use the framebuffer instance with the IPOGLRenderState to render to a texture
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures) = 0;
	
	/*!
		\brief Creates a framebuffer that renders to the supplied textures
		
		\param textures
				The textures we want to use for the framebuffer. The array must end with a nullptr or 0. 
				{@code
					IPOGLTexture** textures = {texture1, texture2, nullptr};
					context->CreateFramebuffer(textures);
				}
		\param depthStencilTexture
				A depth- and/or stencil texture
		\return A framebuffer instance. You can use the framebuffer instance with the IPOGLRenderState to render to a texture
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthStencilTexture) = 0;

	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param layout
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates an index buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param type
		\param bufferUsage
	*/
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Apply the supplied effect to the current context

		\param effect
		\return
	*/
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect) = 0;

	/*!
		\brief Map the data to a memory location and return a pointer to it
	*/
	virtual void* Map(IPOGLResource* resource, POGLResourceMapType::Enum e) = 0;
	
	/*!
		\brief Map parts of the data to a memory location and return a pointer to it
		
		\param offset
				The offset, in bytes, where we want to start saving the data
		\param length
				The length of the data buffer we want to map
	*/
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e) = 0;

	/*!
		\brief Close the stream

		This must be done after all the operations have been completed. This unmaps potentially mapped resources and
		update your objects synchronization status so that any future rending operations will get the correct buffer data.
	*/
	virtual void Unmap(IPOGLResource* resource) = 0;

	/*!
		\brief A rectangle specifying the viewport area

		\param viewport
	*/
	virtual void SetViewport(const POGL_RECT& viewport) = 0;
};

/*!

*/
class IPOGLDeferredDeviceContext : public IPOGLDeviceContext
{
public:
	/*!
		\brief Execute the commands generated by this context and clears the command queue

		\param context
				The context we want to execute the commands in
	*/
	virtual void ExecuteCommands(IPOGLDeviceContext* context) = 0;

	/*!
		\brief Execute the commands generated by this device context

		\param context
				The context we want to execute the commands in
		\param clearCommands
				Boolean value indicating if we should clear the command queue or not. You can invoke this method multiple times and get the
				same result each time if you do not clear the command queue.
	*/
	virtual void ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands) = 0;

	/*!
		\brief Flush this command queue 

		Use this method when all the calls are complete for this context. 
	*/
	virtual void Flush() = 0;
};

/*!
	\brief The sampler state
*/
class IPOGLSamplerState
{
public:
	virtual ~IPOGLSamplerState() {}

	/*!
		\brief Set the min filter

		\param minFilter
	*/
	virtual void SetMinFilter(POGLMinFilter::Enum minFilter) = 0;
	
	/*!
		\brief Set the Mag Filter

		\param minFilter
	*/
	virtual void SetMagFilter(POGLMagFilter::Enum magFilter) = 0;

	/*!
		\brief Set the texture warpping
	*/
	virtual void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t) = 0;
	
	/*!
		\brief Set the texture warpping
	*/
	virtual void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r) = 0;
	
	/*!
		\brief Set the compare function
	*/
	virtual void SetCompareFunc(POGLCompareFunc::Enum compareFunc) = 0;

	/*!
		\brief Set the compare mode
	*/
	virtual void SetCompareMode(POGLCompareMode::Enum compareMode) = 0;
};

/*!
	\brief
*/
class IPOGLUniform
{
public:
	virtual ~IPOGLUniform() {}

	virtual void SetInt32(POGL_INT32 a) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d) = 0;
	virtual void SetInt32(POGL_INT32* ptr, POGL_UINT32 count) = 0;

	virtual void SetUInt32(POGL_UINT32 a) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d) = 0;
	virtual void SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count) = 0;

	virtual void SetFloat(POGL_FLOAT a) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d) = 0;
	virtual void SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count) = 0;

	virtual void SetDouble(POGL_DOUBLE a) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d) = 0;
	virtual void SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count) = 0;

	virtual void SetMatrix(const POGL_MAT4& mat4) = 0;

	virtual void SetVector2(const POGL_VECTOR2& vec) = 0;
	virtual void SetVector3(const POGL_VECTOR3& vec) = 0;
	virtual void SetVector4(const POGL_VECTOR4& vec) = 0;

	virtual IPOGLSamplerState* GetSamplerState() = 0;

	virtual void SetTexture(IPOGLTexture* texture) = 0;
};

/*!
	\brief
*/
class IPOGLShaderProgram : public IPOGLResource
{
public:
};

/*!
	\brief
*/
class IPOGLEffect : public IPOGLResource
{
public:
	/*!
		\brief
	*/
	virtual bool GetDepthTest() = 0;

	/*!
		\brief Should the effect have depth testing enabled.

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthTest(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetDepthTest(bool b) = 0;
	
	/*!
		\brief

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthTest(bool) instead.

		\param depthFunc
	*/
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc) = 0;

	/*!
		\brief
	*/
	virtual POGLDepthFunc::Enum GetDepthFunc() = 0;
	
	/*!
		\brief
	*/
	virtual bool GetDepthMask() = 0;

	/*!
		\brief 

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthMask(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetDepthMask(bool b) = 0;
	
	/*!
		\brief
	*/
	virtual POGL_UINT8 GetColorMask() = 0;

	/*!
		\brief
	*/
	virtual void SetColorMask(POGL_UINT8 colorMask) = 0;

	/*!
		\brief
	*/
	virtual bool GetStencilTest() = 0;

	/*!
		\brief 

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetStencilTest(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetStencilTest(bool b) = 0;

	/*!
		\brief Set the blending factor

		\param sfactor
		\param dfactor
	*/
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor) = 0;

	/*!
		\brief
	*/
	virtual void SetBlend(bool b) = 0;
};

/*!
	\brief
*/
class IPOGLRenderState : public IPOGLInterface
{
public:
	/*!
		\brief Clears the current render states back buffers

		\param clearBits
	*/
	virtual void Clear(POGL_UINT32 clearBits) = 0;

	/*!
		\brief Locate the uniform with the given name
	*/
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name) = 0;

	/*!
		\brief Set the framebuffer used when render to this frame
	*/
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer) = 0;

	/*!
		\brief Draw the entire vertex buffer

		\param vertexBuffer
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param startIndex
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param startIndex
		\param count
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count) = 0;

	/*!
		\brief
	*/
	virtual void SetDepthTest(bool b) = 0;

	/*!
		\brief
	*/
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc) = 0;
	
	/*!
		\brief
	*/
	virtual void SetDepthMask(bool b) = 0;

	/*!
		\brief
	*/
	virtual void SetColorMask(POGL_UINT8 colorMask) = 0;
	
	/*!
		\brief 

		\param b
			TRUE or FALSE
	*/
	virtual void SetStencilTest(bool b) = 0;
	
	/*!
		\brief
	*/
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor) = 0;

	/*!
		\brief
	*/
	virtual void SetBlend(bool b) = 0;
	
	/*!
		\brief A rectangle specifying the viewport area

		\param viewport
	*/
	virtual void SetViewport(const POGL_RECT& viewport) = 0;
};

/*!
	\brief The default texture interface
*/
class IPOGLTexture : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the texture color format for this texture
	*/
	virtual POGLTextureFormat::Enum GetTextureFormat() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture1D : public IPOGLTexture
{
public:
	/*!
		\brief Retrieves the size (width) of this texture in pixels
	*/
	virtual POGL_UINT32 GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture2D : public IPOGLTexture
{
public:
	/*!
		\brief Retrieves the size (width and height) of this texture in pixels
	*/
	virtual const POGL_SIZE& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture3D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetDepth() const = 0;
	
	/*!
		\brief Retrieves the size (width and height) of this texture in pixels
	*/
	virtual const POGL_SIZE& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLFramebuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieve the texture bound at the supplied index

		\param idx
		\return
	*/
	virtual IPOGLTexture* GetTexture(POGL_UINT32 idx) = 0;

	/*!
		\brief Retrieves a depth- and/or stencil texture

		\return
	*/
	virtual IPOGLTexture* GetDepthStencilTexture() = 0;
};

/*!
	\brief
*/
class IPOGLVertexBuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the layout for this buffer

		\return
	*/
	virtual const POGL_VERTEX_LAYOUT* GetLayout() const = 0;

	/*!
		\brief Retrieves the number of vertices located in this buffer
	*/
	virtual POGL_UINT32 GetCount() const = 0;
};

/*!
	\brief
*/
class IPOGLIndexBuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the number of elements found in this buffer
	*/
	virtual POGL_UINT32 GetCount() const = 0;
};

/*!
	\brief Creates a new device instance.

	\param deviceInfo
	\return
*/
extern IPOGLDevice* POGLCreateDevice(const POGL_DEVICE_INFO* info);

//
// Exceptions
//

class POGLException {
public:
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file);
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLException();

	const POGL_CHAR* GetMessage() const;
	const POGL_CHAR* GetFunction() const;
	const POGL_CHAR* GetFile() const;
	POGL_UINT64 GetLine() const;

protected:
	POGL_CHAR mFunction[128];
	POGL_CHAR mFile[256];
	POGL_CHAR mMessage[5096];
	POGL_UINT64 mLine;
};

/*!
	\brief Exception thrown if you are trying to use a feature that's not implemented yet
*/
class POGLNotImplementedException : public POGLException {
public:
	POGLNotImplementedException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLNotImplementedException();
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLInitializationException : public POGLException {
public:
	POGLInitializationException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLInitializationException();
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLResourceException : public POGLException {
public:
	POGLResourceException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLResourceException();
};

/*!
	\brief Exception thrown if a fatal fault happened when initializing or using an effect
*/
class POGLEffectException : public POGLException {
public:
	POGLEffectException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLEffectException();
};

/*!
	\brief Exception thrown if a fatal fault happened when using a state in a way that's not solvable by the rendering engine
*/
class POGLStateException : public POGLException {
public:
	POGLStateException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLStateException();
};

#include <cstdarg>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, POGL_TOCHAR(Message), __VA_ARGS__)
#endif

#ifndef THROW_NOT_IMPLEMENTED_EXCEPTION
#define THROW_NOT_IMPLEMENTED_EXCEPTION() throw POGLNotImplementedException(__FUNCTION__, __LINE__, __FILE__, POGL_TOCHAR("Feature not implemented yet!"))
#endif

#ifndef assert_with_message
#ifdef _DEBUG
#define assert_with_message(test, message) if(!(test)) THROW_EXCEPTION(POGLException, message)
#else
#define assert_with_message(test, message)
#endif
#endif

#ifndef assert_not_null
#ifdef _DEBUG
#define assert_not_null(param) if(param == nullptr) THROW_EXCEPTION(POGLException, "Parameter "## #param ## " cannot be nullptr")
#else
#define assert_not_null(param)
#endif
#endif

#ifndef assert_null
#ifdef _DEBUG
#define assert_null(param) if(param != nullptr) THROW_EXCEPTION(POGLException, "Parameter "## #param ## " is expected be nullptr")
#else
#define assert_null(param)
#endif
#endif

#endif
