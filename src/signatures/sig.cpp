#include "sig.h"
#include "../sigscan/sigscan.h"
#include "../utils/constants.h"
#include "../utils/stb.hh"

#define SDK_SIG(sig) stb::simple_conversion::build<stb::fixed_string{sig}>::value

namespace signatures {
    CSigScan GlowManager("GlowManager", CConstants::CLIENT_LIB,
                            {
                                {SDK_SIG("A1 ? ? ? ? A8 01 75 4B"), [](CPointer& ptr) { ptr.Offset(1); ptr.Dereference(1); ptr.Offset(4);}} 
                            });  
}  