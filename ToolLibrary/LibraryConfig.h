#pragma once

#pragma warning(disable : 4267 4244 4554 4477 4018 4244 4267 4554 6387 4099 4227 4003 26451 26495 6237 4838 26439 4002 4005 4312 4311 6297 6385 33010 26813 6011 6297 6386 4319 4302 28251 28196 4595)

#include <cstdint>
#include <string>
#include <memory>
#include <unistd.h>

/**
 * Library version.
 */
#define _VERSION "8.0.0"

enum class DataStreamMode : unsigned char {
	eMode_Unset = 0,
	eMode_Read = 1,
	eMode_Write = 2
};

//Windows platform definitions
#if defined(_MSC_VER) || defined(__WINE__)

// ============================================================= PLATFORM: WINDOWS =============================================================

//Debug/Release macros
#ifdef _DEBUG
#define DEBUGMODE
#else
#define RELEASEMODE
#endif

#include <windows.h>

//Define to empty file handle for constructor in data stream disc.
#define EMPTY_FILE_HANDLE (HANDLE)INVALID_HANDLE_VALUE

//File handle type
typedef HANDLE FileHandle;

//Windows internal open file
HANDLE openfile_s_(const char* fp, const char* m);

//Define this to truncate the given file handle
#define PlatformSpecTrunc(handle, newsize) SetEndOfFile((HANDLE)handle)

//Define this to open the given file from mode
#define PlatformSpecOpenFile(file_path,mode) openfile_s_(file_path,mode == DataStreamMode::eMode_Write ? "wb" : "rb")

//Define this to close the given file
#define PlatforSpecCloseFile(handle) if(handle != INVALID_HANDLE_VALUE) { CloseHandle(handle); }

#define PlatformLoadLibrary(name) LoadLibraryA(name)

#define PlatformFreeLibrary(libHandle) FreeLibrary(libHandle)

typedef HMODULE LibraryHandle;

#define EMPTY_LIBRARY_HANDLE (HMODULE)0

#define PLATFORM_DYLIB_EXT "dll"

 /**
  * Shortcut for inline spec. Per platform may be different.
  */
#define INLINE __inline

  /**
   * Shortcut for force inline spec.
   */
#define FORCE_INLINE __forceinline

   /**
	* Export to dynamic library.
	*/
#define _TTToolLib_Exp extern "C" __declspec(dllexport)

	/**
	 * Same as _TTToolLib_Exp
	 */
#define TTEXPORT _TTToolLib_Exp

// ============================================================ END PLATFORM: WINDOWS ==========================================================

#elif defined(_POSIX_VERSION)

//Debug/Release macros
#ifdef _DEBUG
#define DEBUGMODE
#else
#define RELEASEMODE
#endif
#include <stdio.h>
#include <dlfcn.h>

//Define to empty file handle for constructor in data stream disc.
#define EMPTY_FILE_HANDLE NULL

//File handle type
typedef FILE* FileHandle;

//Define this to truncate the given file handle
#define PlatformSpecTrunc(handle, newsize) ftruncate(fileno(handle),newsize)

//Define this to open the given file from mode
#define PlatformSpecOpenFile(file_path,mode) fopen(file_path,mode == DataStreamMode::eMode_Write ? "wb" : "rb")

//Define this to close the given file
#define PlatforSpecCloseFile(handle) if(handle) fclose(handle);

#define PlatformLoadLibrary(name) dlopen(name,RTLD_NOW)

#define PlatformFreeLibrary(libHandle) dlclose(libHandle)

typedef void* LibraryHandle;

#define EMPTY_LIBRARY_HANDLE NULL

#define PLATFORM_DYLIB_EXT "so"

 /**
  * Shortcut for inline spec. Per platform may be different.
  */
#define INLINE inline

  /**
   * Shortcut for force inline spec.
   */
#define FORCE_INLINE always_inline

   /**
	* Export to dynamic library.
	*/
#define _TTToolLib_Exp extern "C"

	/**
	 * Same as _TTToolLib_Exp
	 */
#define TTEXPORT _TTToolLib_Exp

// ============================================================ END PLATFORM: WINDOWS ==========================================================

#else

#error "Unknown platform. Please implement the same macros as windows for this platform."

#endif

typedef std::make_unsigned<int64_t>::type u64;
typedef std::make_unsigned<int32_t>::type u32;
typedef std::make_unsigned<int16_t>::type u16;
typedef std::make_unsigned<int8_t> ::type u8;
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char>> String;

inline void* operator new(size_t s){
	if (!s)
		s++;
	return calloc(1, s);
}

inline void* operator new[](size_t s){
	if (!s)
		s++;
	return calloc(1, s);
}

inline void operator delete(void* ptr) noexcept {
	if (ptr)
		free(ptr);
}

inline void operator delete[](void* ptr) noexcept {
	if (ptr)
		free(ptr);
}
