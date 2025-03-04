#ifdef TRACY_ENABLE

#include <tracy/Tracy.hpp>


#define __ZoneScoped                    ZoneScoped
#define __ZoneScopedN(name)             ZoneScopedN(name)
#define __ZoneScopedC(color)            ZoneScopedC(color) 
#define __ZoneScopedNC(name, color)     ZoneScopedNC(name, color)
#define __TracyMessageL(text)           TracyMessageL(text)
#define __TracyMessageLC(text, color)   TracyMessageLC(text, color)


#else

#define __ZoneScoped                   
#define __ZoneScopedN(name)           
#define __ZoneScopedC(color)           
#define __ZoneScopedNC(name, color)    
#define __TracyMessageL(text)          
#define __TracyMessageLC(text, color)  


#endif