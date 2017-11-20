//
// gsl-lite is based on GSL: Guideline Support Library.
// For more information see https://github.com/martinmoene/gsl-lite
//
// Copyright (c) 2015 Martin Moene
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "gsl-lite.t.h"

#include <sstream>  // std::ostringstream
#include <string.h> // strlen()
#include <wchar.h>  // wcslen()

typedef string_span::index_type index_type;

CASE( "string_span: Disallows construction of a string_span from a cstring_span (define gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS)" )
{
#if gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS
    cstring_span sv = "hello";

    string_span v2 = sv;
#endif
}

CASE( "string_span: Disallows construction of a string_span from a const std::string (define gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS)" )
{
#if gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS
    const std::string s = "hello, world";

    string_span sv( s );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
#endif
}

CASE( "string_span: Allows to default-construct" )
{
    string_span s;

    EXPECT( s.size() == index_type( 0 ) );
}

CASE( "string_span: Allows to construct from a nullptr (C++11)" )
{
#if gsl_HAVE_NULLPTR
    string_span  v( nullptr );
    cstring_span w( nullptr );

    EXPECT( v.size() == index_type( 0 ) );
    EXPECT( w.size() == index_type( 0 ) );
#else
    EXPECT( !!"nullptr is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a const C-string" )
{
    cstring_span sv = "hello";

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) ==  "hello" );
}

CASE( "string_span: Allows to construct a string_span from a non-const C-string and size" )
{
    char s[] = "hello";

    string_span sv( s, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a string_span from a non-const C-string begin and end pointer" )
{
    char s[] = "hello";

    string_span sv( s, s + strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a string_span from a non-const C-array" )
{
    char s[] = { 'w', 'o', 'r', 'l', 'd', '\0' };

    string_span sv( s );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == "world" );
}

CASE( "string_span: Allows to construct a string_span from a non-const std::string" )
{
    std::string s = "hello, world";

    string_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a string_span from a non-const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    std::array<char,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    string_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a string_span from a non-const container (std::vector)" )
{

#if gsl_HAVE_INITIALIZER_LIST
    std::vector<char> vec = { 'w', 'o', 'r', 'l', 'd', '\0' };
#else
    std::vector<char> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    string_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a string_span from a non-const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    std::vector<char> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<char> vec; {for ( char i = 1; i < 10; ++i ) vec.push_back(i); }
#endif
    string_span  sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to construct a cstring_span from a non-const C-string and size" )
{
    char s[] = "hello";

    cstring_span sv( s, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cstring_span from a non-const C-string begin and end pointer" )
{
    char s[] = "hello";

    cstring_span sv( s, s + strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cstring_span from a non-const C-array" )
{
    char s[] = { 'w', 'o', 'r', 'l', 'd', '\0' };

    cstring_span sv( s );

#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_COMPILER_MSVC_VERSION
    EXPECT( sv.length() == index_type( 5 ) );
#elif gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    EXPECT( !!"Warning: Unconstrained span container constructor enabled: terminating '\0' included in span" );
    EXPECT( sv.length() == index_type( 6 ) );
#else
    EXPECT( sv.length() == index_type( 5 ) );
#endif
    EXPECT( std::string( sv.data() ) == "world" );
}

CASE( "string_span: Allows to construct a cstring_span from a non-const std::string" )
{
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    std::string s = "hello, world";

    cstring_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::string( sv.data() ) == s );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a non-const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    std::array<char,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    cstring_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a non-const container (std::vector)" )
{
#if gsl_HAVE_INITIALIZER_LIST
    std::vector<char> vec = { 'w', 'o', 'r', 'l', 'd', '\0' };
#else
    std::vector<char> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    cstring_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a non-const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    std::vector<char> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<char> vec; {for ( char i = 1; i < 10; ++i ) vec.push_back(i); }
#endif
    cstring_span sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to construct a cstring_span from a const C-string and size" )
{
    const char s[] = "hello";

    cstring_span sv( s, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cstring_span from a non-const C-string begin and end pointer" )
{
    const char s[] = "hello";

    cstring_span sv( s, s + strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cstring_span from a const C-array" )
{
    const char s[] = { 'w', 'o', 'r', 'l', 'd', '\0' };

    cstring_span sv( s );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == "world" );
}

CASE( "string_span: Allows to construct a cstring_span from a const std::string" )
{
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    const std::string s = "hello, world";

    cstring_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::string( sv.data() ) == s );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    const std::array<char,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    cstring_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a const container (std::vector)" )
{
#if gsl_HAVE_INITIALIZER_LIST
    const std::vector<char> vec = { 'w', 'o', 'r', 'l', 'd', '\0' };
#else
    const std::vector<char> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    cstring_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cstring_span from a const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    const std::vector<char> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<char> tmp; {for ( char i = 1; i < 10; ++i ) tmp.push_back(i); }
    const std::vector<char> vec( tmp );
#endif
    cstring_span sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to construct a wstring_span from a non-const C-string and size" )
{
    wchar_t s[] = L"hello";

    wstring_span sv( s, wcslen( s ) );

    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a wstring_span from a non-const C-string begin and end pointer" )
{
    wchar_t s[] = L"hello";

    wstring_span sv( s, s + wcslen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a wstring_span from a non-const C-array" )
{
    wchar_t s[] = { L'w', L'o', L'r', L'l', L'd', L'\0' };

    wstring_span sv( s );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a wstring_span from a non-const std::wstring" )
{
    std::wstring s = L"hello, world";

    wstring_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a wstring_span from a non-const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    std::array<wchar_t,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    wstring_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a wstring_span from a non-const container (std::vector)" )
{
#if gsl_HAVE_INITIALIZER_LIST
    std::vector<wchar_t> vec = { L'w', L'o', L'r', L'l', L'd', L'\0' };
#else
    std::vector<wchar_t> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    wstring_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a wstring_span from a non-const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    std::vector<wchar_t> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<wchar_t> vec; {for ( wchar_t i = 1; i < 10; ++i ) vec.push_back(i); }
#endif
    wstring_span sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const C-string and size" )
{
    wchar_t s[] = L"hello";

    cwstring_span sv( s, wcslen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const C-string begin and end pointer" )
{
    wchar_t s[] = L"hello";

    cwstring_span sv( s, s + wcslen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const C-array" )
{
    wchar_t s[] = { L'w', L'o', L'r', L'l', L'd', L'\0' };

    cwstring_span sv( s );

#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_COMPILER_MSVC_VERSION
    EXPECT( sv.length() == index_type( 5 ) );
#elif gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    EXPECT( !!"Warning: Unconstrained span container constructor enabled: terminating '\0' included in span" );
    EXPECT( sv.length() == index_type( 6 ) );
#else
    EXPECT( sv.length() == index_type( 5 ) );
#endif
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const std::wstring" )
{
    std::wstring s = L"hello, world";

    cwstring_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    std::array<wchar_t,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    cwstring_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const container (std::vector)" )
{
#if gsl_HAVE_INITIALIZER_LIST
    std::vector<wchar_t> vec = { L'w', L'o', L'r', L'l', L'd', L'\0' };
#else
    std::vector<wchar_t> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    cwstring_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cwstring_span from a non-const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    std::vector<wchar_t> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<wchar_t> vec; {for ( wchar_t i = 1; i < 10; ++i ) vec.push_back(i); }
#endif
    cwstring_span sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a const C-string and size" )
{
    const wchar_t s[] = L"hello";

    cwstring_span sv( s, wcslen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a const C-string begin and end pointer" )
{
    const wchar_t s[] = L"hello";

    cwstring_span sv( s, s + wcslen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cwstring_span from a const C-array" )
{
    const wchar_t s[] = { L'w', L'o', L'r', L'l', L'd', L'\0' };

    cwstring_span sv( s );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "string_span: Allows to construct a cwstring_span from a const std::wstring" )
{
    const std::wstring s = L"hello, world";

    cwstring_span sv( s );

    EXPECT( sv.length() == 12u );
    EXPECT( std::wstring( sv.data() ) == s );
}

CASE( "string_span: Allows to construct a cwstring_span from a const std::array (C++11)" )
{
#if gsl_HAVE_ARRAY
    const std::array<wchar_t,6> arr = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};

    cwstring_span sv( arr );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to construct a cwstring_span from a const container (std::vector)" )
{
#if gsl_HAVE_INITIALIZER_LIST
    const std::vector<wchar_t> vec = { L'w', L'o', L'r', L'l', L'd', L'\0' };
#else
    const std::vector<wchar_t> vec( 6, 'w' );
#endif
#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR || gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    cwstring_span sv( vec );

    EXPECT( sv.length() == index_type( 6 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "string_span: Allows to construct a cwstring_span from a const container, via a tag (std::vector)" )
{
# if gsl_HAVE_INITIALIZER_LIST
    const std::vector<wchar_t> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<wchar_t> tmp; {for ( wchar_t i = 1; i < 10; ++i ) tmp.push_back(i); }
    const std::vector<wchar_t> vec( tmp );
#endif
    cwstring_span sv( with_container, vec );

    EXPECT( sv.length() == index_type( 9 ) );
    EXPECT( std::equal( sv.begin(), sv.end(), vec.begin() ) );
}

CASE( "string_span: Allows to copy-construct from another span of the same type" )
{
    cstring_span a = "hello";

    cstring_span b( a );

    EXPECT( a.length() == b.length() );
    EXPECT( std::equal( a.begin(), a.end(), b.begin() ) );
}

CASE( "string_span: Allows to copy-construct from another span of a compatible type" )
{
    char hello[] = "hello";
    string_span a = hello;

    cstring_span b( a );

    EXPECT( a.length() == b.length() );
    EXPECT( std::equal( a.begin(), a.end(), b.begin() ) );
}

CASE( "string_span: Allows to move-construct from another span of the same type (C++11)" )
{
#if gsl_CPP11_OR_GREATER
    char hello[] = "hello";

    string_span a(( string_span( hello ) ));

    EXPECT( a.length() == index_type( strlen( hello ) ) );
    EXPECT( std::equal( a.begin(), a.end(), hello ) );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to copy-assign from another span of the same type" )
{
    char hello[] = "hello";
    string_span a = hello;
    string_span b;

    b = a;

    EXPECT( a.length() == b.length() );
    EXPECT( std::equal( a.begin(), a.end(), b.begin() ) );
}

CASE( "string_span: Allows to move-assign from another span of the same type (C++11)" )
{
#if gsl_CPP11_OR_GREATER
    char hello[] = "hello";
    string_span a;

    a = string_span( hello );

    EXPECT( a.length() == index_type( strlen( hello ) ) );
    EXPECT( std::equal( a.begin(), a.end(), hello ) );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "string_span: Allows to create a sub span of the first n elements" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type count = 2;

    string_span s = a.first( count );

    EXPECT( s.size() == count );
    EXPECT( std::equal( s.begin(), s.end(), hello ) );
}

CASE( "string_span: Allows to create a sub span of the last n elements" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type count = 2;

    string_span s = a.last( count );

    EXPECT( s.size() == count );
    EXPECT( std::equal( s.begin(), s.end(), hello + strlen(hello) - count ) );
}

CASE( "string_span: Allows to create a sub span starting at a given offset" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type offset = 1;

    string_span s = a.subspan( offset );

    EXPECT( s.size() == index_type( strlen(hello) - offset ) );
    EXPECT( std::equal( s.begin(), s.end(), hello + offset ) );
}

CASE( "string_span: Allows to create a sub span starting at a given offset with a given length" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type offset = 1;
    index_type length = 1;

    string_span s = a.subspan( offset, length );

    EXPECT( s.size() == length );
    EXPECT( std::equal( s.begin(), s.end(), hello + offset ) );
}

CASE( "string_span: Allows to create an empty sub span at full offset" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type offset = strlen( hello );

    string_span s = a.subspan( offset );

    EXPECT( s.empty() );
}

CASE( "string_span: Allows to create an empty sub span at full offset with zero length" )
{
    char hello[] = "hello";
    string_span a( hello );
    index_type offset = strlen( hello );
    index_type length = 0;

    string_span s = a.subspan( offset, length );

    EXPECT( s.empty() );
}

CASE( "string_span: Allows forward iteration" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( string_span::iterator pos = a.begin(); pos != a.end(); ++pos )
    {
        index_type i = narrow<index_type>( std::distance( a.begin(), pos ) );
        EXPECT( *pos == a[i] );
    }
}

CASE( "string_span: Allows const forward iteration" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( string_span::const_iterator pos = a.begin(); pos != a.end(); ++pos )
    {
        index_type i = narrow<index_type>( std::distance( a.cbegin(), pos ) );
        EXPECT( *pos == a[i] );
    }
}

CASE( "string_span: Allows reverse iteration" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( string_span::reverse_iterator pos = a.rbegin(); pos != a.rend(); ++pos )
    {
        index_type dist = narrow<index_type>( std::distance( a.rbegin(), pos ) );
        EXPECT( *pos == a[ a.size() - 1 - dist ] );
    }
}

CASE( "string_span: Allows const reverse iteration" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( string_span::const_reverse_iterator pos = a.crbegin(); pos != a.crend(); ++pos )
    {
        index_type dist = narrow<index_type>( std::distance( a.crbegin(), pos ) );
        EXPECT( *pos == a[ a.size() - 1 - dist ] );
    }
}

CASE( "string_span: Allows to observe an element via array indexing" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( index_type i = 0; i < a.size(); ++i )
    {
        EXPECT( a[i] == hello[i] );
    }
}

CASE( "string_span: Allows to observe an element via call indexing" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( index_type i = 0; i < a.size(); ++i )
    {
        EXPECT( a(i) == hello[i] );
    }
}

#if 0
CASE( "string_span: Allows to observe an element via at()" )
{
    char hello[] = "hello";
    string_span a( hello );

    for ( index_type i = 0; i < a.size(); ++i )
    {
        EXPECT( a.at(i) == hello[i] );
    }
}
#endif

CASE( "string_span: Allows to observe an element via data()" )
{
    char hello[] = "hello";
    string_span a( hello );

    EXPECT( *a.data() == *a.begin() );

    for ( index_type i = 0; i < a.size(); ++i )
    {
        EXPECT( a.data()[i] == hello[i] );
    }
}

CASE( "string_span: Allows to change an element via array indexing" )
{
    char hello[] = "hello";
    string_span a( hello );

    a[1] = '7';

    EXPECT( hello[1] == '7' );
}

CASE( "string_span: Allows to change an element via call indexing" )
{
    char hello[] = "hello";
    string_span a( hello );

    a(1) = '7';

    EXPECT( hello[1] == '7' );
}

#if 0
CASE( "string_span: Allows to change an element via at()" )
{
    char hello[] = "hello";
    string_span a( hello );

    a.at(1) = '7';

    EXPECT( hello[1] == '7' );
}
#endif

CASE( "string_span: Allows to change an element via data()" )
{
    char hello[] = "hello";
    string_span a( hello );

    *a.data() = '7';

    EXPECT( hello[0] == '7' );
}

CASE( "string_span: Allows to compare a string_span with another string_span" )
{
    char s[] = "hello";
    char t[] = "world";

    string_span sv( s, strlen( s ) );
    string_span tv( t, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( tv.length() == index_type( 5 ) );

    EXPECT( sv == sv );
    EXPECT( sv != tv );
    EXPECT( sv <= sv );
    EXPECT( sv <= tv );
    EXPECT( sv <  tv );
    EXPECT( tv >= tv );
    EXPECT( tv >= sv );
    EXPECT( tv >  sv );
}

CASE( "string_span: Allows to compare empty spans as equal" )
{
    string_span a, b;

    EXPECT( a == b );
}

CASE( "string_span: Allows to compare a string_span with a cstring_span" )
{
#if gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON
    char s[] = "hello";

    string_span   sv( s, strlen( s ) );
    cstring_span csv( s, strlen( s ) );

    EXPECT( sv == csv );
#else
    EXPECT( !!"string_span: cannot compare different types (gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON=0)" );
#endif
}

CASE( "string_span: Allows to compare with types convertible to string_span" )
{
#if gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON

    char const * phello   = "hello";
    char const   ahello[] = "hello";
    std::string  shello   = "hello";
    cstring_span        a = phello;

    EXPECT(     a == phello );
    EXPECT(     a == ahello );
    EXPECT(     a == shello );
    EXPECT(     a <= phello );
    EXPECT(     a <= ahello );
    EXPECT(     a <= shello );
    EXPECT(     a >= phello );
    EXPECT(     a >= ahello );
    EXPECT(     a >= shello );

    EXPECT_NOT( a != phello );
    EXPECT_NOT( a != ahello );
    EXPECT_NOT( a != shello );
    EXPECT_NOT( a  < phello );
    EXPECT_NOT( a  < ahello );
    EXPECT_NOT( a  < shello );
    EXPECT_NOT( a  > phello );
    EXPECT_NOT( a  > ahello );
    EXPECT_NOT( a  > shello );

#if gsl_HAVE_OWNER_TEMPLATE
    EXPECT(     phello == a );
    EXPECT(     ahello == a );
    EXPECT(     shello == a );
    EXPECT(     phello <= a );
    EXPECT(     ahello <= a );
    EXPECT(     shello <= a );
    EXPECT(     phello >= a );
    EXPECT(     ahello >= a );
    EXPECT(     shello >= a );

    EXPECT_NOT( phello != a );
    EXPECT_NOT( ahello != a );
    EXPECT_NOT( shello != a );
    EXPECT_NOT( phello  < a );
    EXPECT_NOT( ahello  < a );
    EXPECT_NOT( shello  < a );
    EXPECT_NOT( phello  > a );
    EXPECT_NOT( ahello  > a );
    EXPECT_NOT( shello  > a );
#endif

#if gsl_HAVE_ARRAY
    std::array<char,6> world = {{ 'w', 'o', 'r', 'l', 'd', '\0' }};
    cstring_span   b = world;

    EXPECT( b == world );
# if gsl_HAVE_OWNER_TEMPLATE
    EXPECT( world == b );
# endif
#endif // gsl_HAVE_OWNER_TEMPLATE
#else
    EXPECT( !!"string_span: cannot compare different types (gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON=0)" );
#endif
}

CASE( "string_span: Allows to test for empty span via empty(), empty case" )
{
    string_span sz;

    EXPECT( sz.empty() );
}

CASE( "string_span: Allows to test for empty span via empty(), non-empty case" )
{
    cstring_span s = "hello";

    EXPECT_NOT( s.empty() );
}

CASE( "string_span: Allows to obtain the number of elements via length()" )
{
    char a[] = "hello";
    char b[] = "world";
    string_span sz;
    string_span sa = a;
    string_span sb = b;

    EXPECT( sz.length() == index_type( 0 ) );
    EXPECT( sa.length() == index_type( strlen( a ) ) );
    EXPECT( sb.length() == index_type( strlen( b ) ) );
}

CASE( "string_span: Allows to obtain the number of elements via size()" )
{
    char a[] = "hello";
    char b[] = "world";
    string_span sz;
    string_span sa = a;
    string_span sb = b;

    EXPECT( sz.size() == index_type( 0 ) );
    EXPECT( sa.size() == index_type( strlen( a ) ) );
    EXPECT( sb.size() == index_type( strlen( b ) ) );
}

CASE( "string_span: Allows to obtain the number of bytes via length_bytes()" )
{
    wchar_t a[] = L"hello";
    wchar_t b[] = L"world";
    wstring_span sz;
    wstring_span sa = a;
    wstring_span sb = b;

    EXPECT( sz.length_bytes() == index_type( 0 ) );
    EXPECT( sa.length_bytes() == index_type( sizeof(wchar_t) * wcslen( a ) ) );
    EXPECT( sb.length_bytes() == index_type( sizeof(wchar_t) * wcslen( b ) ) );
}

CASE( "string_span: Allows to obtain the number of bytes via size_bytes()" )
{
    wchar_t a[] = L"hello";
    wchar_t b[] = L"world";
    wstring_span sz;
    wstring_span sa = a;
    wstring_span sb = b;

    EXPECT( sz.size_bytes() == index_type( 0 ) );
    EXPECT( sa.size_bytes() == index_type( sizeof(wchar_t) * wcslen( a ) ) );
    EXPECT( sb.size_bytes() == index_type( sizeof(wchar_t) * wcslen( b ) ) );
}

CASE( "string_span: Allows to view the elements as read-only bytes" )
{
    char const *    hello = "hello";
    cstring_span s( hello);

    span<const gsl::byte> b = as_bytes( s );

    EXPECT( b[0] == to_byte( hello[0] ) );
    EXPECT( b[1] == to_byte( hello[1] ) );
    EXPECT( b[2] == to_byte( hello[2] ) );
    EXPECT( b[3] == to_byte( hello[3] ) );
    EXPECT( b[4] == to_byte( hello[4] ) );
}

CASE( "to_string(): Allows to explicitly convert from string_span to std::string" )
{
    char s[] = "hello";

    string_span sv( s, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( to_string( sv ) == s );
}

CASE( "to_string(): Allows to explicitly convert from cstring_span to std::string" )
{
    const char s[] = "hello";

    cstring_span sv( s, strlen( s ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( to_string( sv ) == s );
}

CASE( "to_string(): Allows to explicitly convert from wstring_span to std::wstring" )
{
    wchar_t s[] = L"hello";

    wstring_span sv( s, wcslen( s ) );

    std::wstring ws( to_string( wstring_span( s, wcslen( s ) ) ) );

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::equal( ws.begin(), ws.end(), s ) );
}

CASE( "to_string(): Allows to explicitly convert from cwstring_span to std::wstring" )
{
    wchar_t s[] = L"hello";

    std::wstring ws( to_string( cwstring_span( s, wcslen( s ) ) ) );

    EXPECT( ws.length() == 5u );
    EXPECT( std::equal( ws.begin(), ws.end(), s ) );
}

CASE( "ensure_z(): Disallows to build a string_span from a const C-string" )
{
#if gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS
    const char s[] = "hello";

    string_span sv = ensure_z( s );

    EXPECT( sv.length() == 5u );
    EXPECT( std::string( sv.data() ) == s );
#endif
}

CASE( "ensure_z(): Disallows to build a wstring_span from a const wide C-string" )
{
#if gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS
    const wchar_t s[] = L"hello";

    wstring_span sv = ensure_z( s );

    EXPECT( sv.length() == 5 );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
#endif
}

CASE( "ensure_z(): Allows to build a string_span from a non-const C-string" )
{
    char s[] = "hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    string_span sv = ensure_z( s );
#else
    string_span sv = ensure_z( &s[0] );
#endif
    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "ensure_z(): Allows to build a cstring_span from a non-const C-string" )
{
    char s[] = "hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    cstring_span sv = ensure_z( s );
#else
    cstring_span sv = ensure_z( &s[0] );
#endif

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "ensure_z(): Allows to build a cstring_span from a const C-string" )
{
    const char s[] = "hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    cstring_span sv = ensure_z( s );
#else
    cstring_span sv = ensure_z( &s[0] );
#endif

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::string( sv.data() ) == s );
}

CASE( "ensure_z(): Allows to build a wstring_span from a non-const wide C-string" )
{
    wchar_t s[] = L"hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    wstring_span sv = ensure_z( s );
#else
    wstring_span sv = ensure_z( &s[0] );
#endif

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "ensure_z(): Allows to build a cwstring_span from a non-const wide C-string" )
{
    wchar_t s[] = L"hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    cwstring_span sv = ensure_z( s );
#else
    cwstring_span sv = ensure_z( &s[0] );
#endif

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "ensure_z(): Allows to build a cwstring_span from a const wide C-string" )
{
    const wchar_t s[] = L"hello";

#if gsl_COMPILER_MSVC_VERSION != 6
    cwstring_span sv = ensure_z( s );
#else
    cwstring_span sv = ensure_z( &s[0] );
#endif

    EXPECT( sv.length() == index_type( 5 ) );
    EXPECT( std::wstring( sv.data() ) == std::wstring( s ) );
}

CASE( "ensure_z(): Allows to specify ultimate location of the sentinel and ensure its presence" )
{
    const char * s = "hello"; // not: s[]

    EXPECT_THROWS( ensure_z( s, index_type( 3 ) ) );
}

CASE ( "operator<<: Allows printing a string_span to an output stream" )
{
    std::ostringstream oss;
    char s[] = "hello";
    string_span sv = ensure_z( s );

    oss << sv << '\n'
        << std::left << std::setw(10) << sv << '\n'
        << sv << '\n'
        << std::setfill('.') << std::right << std::setw(10) << sv;

    EXPECT( oss.str() == "hello\n     hello\nhello\nhello....." );
}

CASE ( "operator<<: Allows printing a cstring_span to an output stream" )
{
    std::ostringstream oss;
    char s[] = "hello";
    cstring_span sv = ensure_z( s );

    oss << sv << '\n'
        << std::left << std::setw(10) << sv << '\n'
        << sv << '\n'
        << std::setfill('.') << std::right << std::setw(10) << sv;

    EXPECT( oss.str() == "hello\n     hello\nhello\nhello....." );
}

CASE ( "operator<<: Allows printing a wstring_span to an output stream" )
{
    std::wostringstream oss;
    wchar_t s[] = L"hello";
    wstring_span sv = ensure_z( s );

    oss << sv << '\n'
        << std::left << std::setw(10) << sv << '\n'
        << sv << '\n'
        << std::setfill(L'.') << std::right << std::setw(10) << sv;

    EXPECT( oss.str() == L"hello\n     hello\nhello\nhello....." );
}

CASE ( "operator<<: Allows printing a cwstring_span to an output stream" )
{
    std::wostringstream oss;
    wchar_t s[] = L"hello";
    cwstring_span sv = ensure_z( s );

    oss << sv << '\n'
        << std::left << std::setw(10) << sv << '\n'
        << sv << '\n'
        << std::setfill(L'.') << std::right << std::setw(10) << sv;

    EXPECT( oss.str() == L"hello\n     hello\nhello\nhello....." );
}

// end of file
