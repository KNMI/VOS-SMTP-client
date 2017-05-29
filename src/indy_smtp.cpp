//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("indy_smtp_main.cpp", Form1);


AnsiString turbowin_dir;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
   try
   {
      turbowin_dir = lpCmdLine;

      Application->Initialize();
      Application->CreateForm(__classid(TForm1), &Form1);
      Application->Run();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
   }

   return 0;
}
//---------------------------------------------------------------------------
