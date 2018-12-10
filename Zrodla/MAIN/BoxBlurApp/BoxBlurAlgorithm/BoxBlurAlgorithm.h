// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BOXBLURALGORITHM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BOXBLURALGORITHM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BOXBLURALGORITHM_EXPORTS
#define BOXBLURALGORITHM_API __declspec(dllexport)
#else
#define BOXBLURALGORITHM_API __declspec(dllimport)
#endif

// This class is exported from the BoxBlurAlgorithm.dll
class BOXBLURALGORITHM_API CBoxBlurAlgorithm {
public:
	CBoxBlurAlgorithm(void);
	// TODO: add your methods here.
};

extern BOXBLURALGORITHM_API int nBoxBlurAlgorithm;

BOXBLURALGORITHM_API int fnBoxBlurAlgorithm(void);
