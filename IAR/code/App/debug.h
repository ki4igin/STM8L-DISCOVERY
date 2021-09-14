#ifndef __DEBUG_H
#define __DEBUG_H

// Includes --------------------------------------------------------------------

// Typedef ---------------------------------------------------------------------

// Macro -----------------------------------------------------------------------

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
void DebugSendMessage(char *str);
void DebugSendArray(void *pbuf, int len);
void DebugSendWord(int data);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
void _Error_Handler(char *, int);

// Inline Functions ------------------------------------------------------------

#endif
