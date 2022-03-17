# https://cdtdoug.ca/2017/10/06/qnx-cmake-toolchain-file.html
# https://www.qnx.com/support/knowledgebase.html?id=5011O000001OLcS
# https://support7.qnx.com/download/download/26837/Qt_Developers_Guide.pdf
# http://www.qnx.com/developers/docs/6.6.0_anm11_wf10/#com.qnx.doc.neutrino.building/topic/config_Shared_libraries.html
# http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.building/topic/buildfiles/autoso.html



# =====================================
# Available C++ libraries in QCC (CXX):
# =====================================

# <algorithm> · <array> · <bitset> · <cassert> · <ccomplex> · <cctype> · <cerrno> · <cfenv> · <cfloat> · <cinttypes> · <ciso646> · <climits> · <clocale> · <cmath> · <complex> · <csetjmp> · <csignal> · <cstdarg> · <cstdbool> · <cstddef> · <cstdint> · <cstdio> · <cstdlib> · 
# <cstring> · <ctgmath> · <ctime> · <cwchar> · <cwctype> · <deque> · <exception> · <fstream> · <functional> · <hardware> · <hash_map> · <hash_set> · <iomanip> · <ios> · <iosfwd> · <iostream> · <istream> · <iterator> · <limits> · <list> · <locale> · <map> · <memory> · <new> · 
# <numeric> · <ostream> · <queue> · <random> · <regex> · <set> · <sstream> · <stack> · <stdexcept> · <streambuf> · <string> · <strstream> · <tuple> · <typeinfo> · <type_traits> · <unordered_map> · <unordered_set> · <utility> · <valarray> · <vector>






# set(QNX_HOST "/home/kdeoliveira/app/qnx/qnx710/host/linux/x86_64")
# set(QNX_TARGET "/home/kdeoliveira/app/qnx/qnx7")

# set(CMAKE_SYSTEM_NAME QNX)

# set(arch gcc_ntox86_64)
# set(ntoarch x86_64)
# set(QNX_PROCESSOR x86_64)

# set(CMAKE_C_COMPILER qcc)
# set(CMAKE_C_COMPILER_TARGET ${arch})

# set(CMAKE_CXX_COMPILER q++)
# set(CMAKE_CXX_COMPILER_TARGET ${arch})

# set(CMAKE_ASM_COMPILER qcc -V${arch})
# set(CMAKE_ASM_DEFINE_FLAG "-Wa,--defsym,")

# set(CMAKE_RANLIB ${QNX_HOST}/usr/bin/nto${ntoarch}-ranlib
#     CACHE PATH "QNX ranlib Program" FORCE)
# set(CMAKE_AR ${QNX_HOST}/usr/bin/nto${ntoarch}-ar
#     CACHE PATH "QNX qr Program" FORCE)






    # set(CMAKE_SYSTEM_NAME QNX)

    # set(arch gcc_ntox86_64)
    
    # set(CMAKE_C_COMPILER qcc)
    # set(CMAKE_C_COMPILER_TARGET ${arch})
    # set(CMAKE_CXX_COMPILER q++)
    # set(CMAKE_CXX_COMPILER_TARGET ${arch})
    
    # set(CMAKE_SYSROOT ${QNX_TARGET})


set(CMAKE_SYSTEM_NAME QNX)

set(arch gcc_ntox86_64)
set(ntoarch x86_64)
set(QNX_PROCESSOR x86_64)

set(CMAKE_C_COMPILER $ENV{QNX_HOST}/usr/bin/qcc)
set(CMAKE_C_COMPILER_TARGET ${arch})

set(CMAKE_CXX_COMPILER $ENV{QNX_HOST}/usr/bin/qcc -lang-c++)
set(CMAKE_CXX_COMPILER_TARGET ${arch})

set(CMAKE_ASM_COMPILER qcc -V${arch})
set(CMAKE_ASM_DEFINE_FLAG "-Wa,--defsym,")

set(CMAKE_RANLIB $ENV{QNX_HOST}/usr/bin/nto${ntoarch}-ranlib
    CACHE PATH "QNX ranlib Program" FORCE)
set(CMAKE_AR $ENV{QNX_HOST}/usr/bin/nto${ntoarch}-ar
    CACHE PATH "QNX qr Program" FORCE)


add_compile_definitions(_QNX_x86_64=true)


