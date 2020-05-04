#define NUMLINES ((int) (sizeof sysmetrics / sizeof sysmetrics[0]))

struct {
	int iIndex;
	TCHAR* szLabel;
	TCHAR* szDesc;
}

sysmetrics[] = {
	SM_CXSCREEN, TEXT("SM_CXSCREEN"), TEXT("Screen width in pixels"),
	SM_CYCURSOR, TEXT ("SM_CYCURSOR"), TEXT ("Cursor height")
};
