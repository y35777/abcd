#include "ace/OS.h"
#include "ace/Process_Manager.h"
#include "ace/ace.h"
#include "iostream.h"
#include "ace/Process_Mutex.h" 
#include "../../com/include/resource.h"
#include "../../com/include/base_type.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow) 
{
  
    STRING strTitle = S_CLUSTER_EXIT_MUTEX;
    if((GetVersion() & 0xFF) >= 5)
    {
        strTitle = "Global\\" + strTitle;
    }

    ACE_Process_Mutex* t = new ACE_Process_Mutex(strTitle.c_str());

	t->acquire();
	
    ACE_OS::sleep (5);

    delete t;

	return 0;
}
