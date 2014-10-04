14/07/20 by jc

*** Testing Patch Level 5 ***

File: "xbmc_CGraphicContext::ToggleFullScreenRoot_disbaled.bin"
===============================================================

  Install: copy this file to /usr/lib/xbmc/xbmc.bin and change the SHA1 string in  /etc  to 138dfa63eff9df96198c930c36fedc996fa5d976
  Changes:

   - debug messages from patch level four wrapped in '#ifdef RETRORIG_PL4'/'#endif'
   - in xbmc/guilib/GraphicContext.cpp: 
         
       'bool CGraphicContext::ToggleFullScreenRoot ()' now returns immediately, toggle fullscreen is disbaled (bug fix for #68)

  Test instructions: 
   
   - Change display settings from fullscreen to windowed and back
   - Launch some games from RetroRig

  Impact assessment: This change does affect fullscreen commands send to xbmc by external programs like wmctrl. This function was disable.
                     Because RetroRig doesn't need this function it can be disabled.

  On a side note: Launching games from RCB is much faster. XBMC stays in the background, the desktop is not revealed during an emulator start.
                  This is increasing the console feeling.
