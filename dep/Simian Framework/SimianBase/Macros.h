#ifdef __SIMIAN_EXPORTS
	#define SIM_EXP __declspec(dllexport)
#else
	#define SIM_EXP __declspec(dllimport)
#endif

#define SAFE_DELETE(X) { if(X) { delete X; X=0; } }
#define SAFE_DELETEA(X) { if(X) { delete [] X; X=0; } }