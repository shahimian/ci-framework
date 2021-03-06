/*-----------------------------------------------
 SYSMETS.H -- System metrics display structure
 -----------------------------------------------*/
#define NUMLINES ((int) (sizeof sysmetrics / sizeof sysmetrics [0]))
struct
{
 int iIndex ;
 TCHAR * szLabel ;
 TCHAR * szDesc ;
}
sysmetrics [] =
{
 SM_CXSCREEN, TEXT ("SM_CXSCREEN"),
 TEXT ("Screen width in pixels"),
 SM_CYSCREEN, TEXT ("SM_CYSCREEN"),
 TEXT ("Screen height in pixels"),
 SM_CXVSCROLL, TEXT ("SM_CXVSCROLL"),
 TEXT ("Vertical scroll width"),
 SM_CYHSCROLL, TEXT ("SM_CYHSCROLL"),
 TEXT ("Horizontal scroll height"),
 SM_CYCAPTION, TEXT ("SM_CYCAPTION"),
 TEXT ("Caption bar height"),
 SM_CXBORDER, TEXT ("SM_CXBORDER"),
 TEXT ("Window border width"),
 SM_CYBORDER, TEXT ("SM_CYBORDER"),
 TEXT ("Window border height"),
 SM_CXFIXEDFRAME, TEXT ("SM_CXFIXEDFRAME"),
 TEXT ("Dialog window frame width"),
 SM_CYFIXEDFRAME, TEXT ("SM_CYFIXEDFRAME"),
 TEXT ("Dialog window frame height"),
 SM_CYVTHUMB, TEXT ("SM_CYVTHUMB"),
 TEXT ("Vertical scroll thumb height"),
 SM_CXHTHUMB, TEXT ("SM_CXHTHUMB"),
 TEXT ("Horizontal scroll thumb width"),
 SM_CXICON, TEXT ("SM_CXICON"),
 TEXT ("Icon width"),
 SM_CYICON, TEXT ("SM_CYICON"),
 TEXT ("Icon height")};
 
