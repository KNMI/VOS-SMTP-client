//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "indy_smtp_main.h"
#include "fstream.h"                 // ifstream
#include "dir.h"                     // getcwd

extern AnsiString turbowin_dir;



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdMessageClient"
#pragma link "IdSMTP"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma link "IdMessage"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   // n.b. deze const's worden ook in TurboWin main module gebruikt !!!
   // (dus bij veranderen ook daar veranderen)
   //
   const AnsiString text_toaddress     =  "ToAddress         : ";
   const AnsiString text_subject       =  "Subject           : ";
   const AnsiString text_host          =  "Host              : ";
   const AnsiString text_userid        =  "UserID            : ";
   const AnsiString text_fromaddress   =  "FromAddress       : ";
   const AnsiString text_fromname      =  "FromName          : ";
   const AnsiString email_sub_dir      =  "\\email\\";
   const AnsiString emailsettings_file =  "emailsettings.txt";

   // lokale var's
   //
   const int maxlength = 256;
   ifstream is_settings;                           // input file stream
   ifstream is_obs;
   AnsiString record;                              // AnsiString 1-based !!!!!!
   const AnsiString obs_file = "obs.txt";
   //char turbowin_dir[maxlength];
   char char_record[maxlength];
   AnsiString info;
   AnsiString volledig_obs_path;
   AnsiString volledig_emailsettings_path;
   int spatie_teller;
   int line_teller;
   int pos_var_date_time;
   AnsiString var_date_time           = "";

   //noskipws(is);                                  // switch off the automatic skipping of white space characters.



   // omdat vanuit TurboWin opgestart geeft het de home TurboWin directory
   // en NIET de email directory (waar EmailTurboWin.exe in staat)
   //getcwd(turbowin_dir, maxlength);
   //::MessageBox(0, working_dir, "huidige werk dir", MB_OK);


   // files nodig:
   //  - emailsettings.txt   -> hierin de TurboWin E-mail settings (Obs by E-mail(turbo))
   //  - obs.txt             -> hierin de te versturen gecodeerde waarneming



   //
   // TurboWin emailsetings.txt file
   //
   volledig_emailsettings_path = turbowin_dir + email_sub_dir + emailsettings_file;
   is_settings.open(volledig_emailsettings_path.c_str(), ios::in);
   //is_settings.open("C:\\Bc5\\TurboWin\\email\\emailsettings.txt", ios::in);
   if (is_settings)
   {
      //
      // openen obs.txt
      // (de obs is nodig als eerste om eventueel dag-uur er uit te halen voor subject field)
      //
      volledig_obs_path = turbowin_dir + email_sub_dir + obs_file;
      is_obs.open(volledig_obs_path.c_str(), ios::in);
      //is_obs.open("C:\\Bc5\\TurboWin\\email\\obs.txt", ios::in);
      if (is_obs)
      {
         MemoBody -> Lines -> Clear();       // clear MemoBody !! (anders blanko regels er tussen door)

         line_teller = 0;
         do // while (!is.eof())
         {
            is_obs.getline(char_record, 256);
            line_teller++;

            if ((AnsiString)char_record != "")
            {
               MemoBody -> Lines -> Add(char_record);
               //NMSMTP1 -> PostMessage -> Body -> Add(char_record);
               IdMsgSend -> Body -> Add(char_record);

               // dag en uur uit obs halen, kan nodig zijn voor subject field
               if (line_teller == 1)                          // staat in eerste deel (regel) van obs
               {
                  spatie_teller = 0;                          // initialisatie
                  for (int i = 1; i < maxlength; i++)         // AnsiString begint bij 1 (en niet bij 0)
                  {
                     if (((AnsiString)char_record).SubString(i, 1) == " ")   // spatie
                     {
                        spatie_teller++;
                        if (spatie_teller == 2)               // 2 spaties gepasseerd
                        {
                           pos_var_date_time = i + 1;

                           //char buffer[10];
                           //sprintf(buffer, "%d", pos_var_date_time);
                           //MessageBox(0, buffer, "pos_var_date_time", MB_OK);

                           if ((pos_var_date_time + 4) < ((AnsiString)char_record).Length())  // beveiliging
                              var_date_time = ((AnsiString)char_record).SubString(pos_var_date_time, 4) + "00";

                        } // if (spatie_teller == 2)
                     } // if (char_record[i] == " ";
                  } // for (int i = 1; i < max_length; i++)
               } // if (line_teller == 1)
            } // if ((AnsiString)char_record != "")

         } while(!is_obs.eof());

         is_obs.close();
      }  // if (is_obs); obs file
      else // no obs file
      {
         info = "Unable to open obs file (";
         info += volledig_obs_path;
         info += ")";
          ::MessageBox(0, info.c_str(), "TurboWin error", MB_OK);
      } // else (no obs file


      do // while (!is.eof())
      {
         // N.b. onderstaande geeft altijd een record met lengte 0
         //is.getline(record.c_str(), 255);

         is_settings.getline(char_record, 256);               // record line inlezen

         /*
         // To Address (recipient)
         */
         if ((record = (AnsiString)char_record).SubString(1, 20) == text_toaddress)
         {
            if (record.Length() >= 21)
            {
               EditTo -> Text = record.SubString(21, record.Length());
               IdMsgSend -> Recipients -> EMailAddresses = record.SubString(21, record.Length());
            } // if (record.Length() >= 21)
         }

         /*
         // subject
         */
         else if ((record = (AnsiString)char_record).SubString(1, 20) == text_subject)
         {
            if (record.Length() >= 21)
            {
               // eventueel vervangen chars "ddhhmm" door var_date_time (b.v. 271200)
               // n.b. er moet dus geen ddhhmm in de text string zelf (voor de :) voorkomen
               int positie = record.Pos("ddhhmm");
               if (positie != 0)
               {
                  //char buffer[10];
                  //sprintf(buffer, "%d", positie);
                  //MessageBox(0, buffer, "positie index ddhhmm", MB_OK);

                  record.Delete(positie, 6);                  // ddhhmm verwijderen
                  record.Insert(var_date_time, positie);      // b.v. 271200 inserten

               } // if (int positie = record.Pos("ddhhmm") != 0)

               EditSubject -> Text = record.SubString(21, record.Length());
               IdMsgSend -> Subject = record.SubString(21, record.Length()).c_str();

            } // if (record.Length() >= 21)
         }

         /*
         // Host name
         */
         else if ((record = (AnsiString)char_record).SubString(1, 20) == text_host)
         {
            if (record.Length() >= 21)
            {
               SMTP -> Host = record.SubString(21, record.Length()).c_str();
            } // if (record.Length() >= 21)
         }

         /*
         // user ID
         */
         else if ((record = (AnsiString)char_record).SubString(1, 20) == text_userid)
         {
            if (record.Length() >= 21)
            {
               SMTP -> UserId = record.SubString(21, record.Length()).c_str();
            } // if (record.Length() >= 21)
         }

         /*
         // From Address (zie TIdEMailAdressItem.Text in INDY9 help file)
         */
         else if ((record = (AnsiString)char_record).SubString(1, 20) == text_fromaddress)
         {
            if (record.Length() >= 21)
            {
               //IdMsgSend -> From -> Text = record.SubString(21, record.Length()).c_str();
               IdMsgSend -> From -> Address = record.SubString(21, record.Length()).c_str();
            } // if (record.Length() >= 21)
         }

         /*
         // from name (zie TIdEMailAdressItem.Text in INDY9 help file)
         */
         else if ((record = (AnsiString)char_record).SubString(1, 20) == text_fromname)
         {
            if (record.Length() >= 21)
            {
               IdMsgSend -> From -> Name = record.SubString(21, record.Length()).c_str();
            } // if (record.Length() >= 21)
         }

         /*
         // Receipt recipient
         */
         // altijd blanco (nooit een return message
         IdMsgSend -> ReceiptRecipient -> Text = "";;


         /*
         // altijd 25 voor internet SMTP poort
         */
         SMTP -> Port = StrToInt("25");

      } while (!is_settings.eof());

      is_settings.close();
   } // if (is_settings); e-mail settings file
   else // no E-mail settings file
   {
      info = "Unable to open TurboWin E-mail settings file (";
      info += volledig_emailsettings_path;
      info += ").";
      info += " Please select: Maintenance -> E-mail settings.";
      ::MessageBox(0, info.c_str(), "TurboWin error", MB_OK);
   } // else (no E-mail settings file)
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ButtonSendClick(TObject *Sender)
{
   //StatusBar1 -> SimpleText = "Sending...";  // niet doen omdat bij OnStatus komt hierna connected
   Send_SMTP();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnStatus(TObject *axSender, const TIdStatus axStatus, const AnsiString asStatusText)
{
   StatusBar1 -> SimpleText = asStatusText;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SMTPDisconnected(TObject *Sender)
{
   // schonen statusbar (anders blijft er staan "not connected")
   StatusBar1 -> SimpleText = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CancelClick(TObject *Sender)
{
   StatusBar1 -> SimpleText = "Cancelled";
   Close();
}
//---------------------------------------------------------------------------


void TForm1::Send_SMTP()
{
   AnsiString info;
   bool obs_sent_ok = true;


   try
   {
      SMTP -> Connect();
      SMTP -> Send(IdMsgSend);
      //StatusBar1 -> SimpleText = "Obs sent succesfully";
      // nb er is ook een QuickSend
   }
   catch(...)
   //catch(Exception &E)
   {
      SMTP -> Disconnect();
      // Exception on resolving domain
      //ShowMessage("failed");
      //Application -> ShowException(&E);
      obs_sent_ok = false;
      StatusBar1 -> SimpleText = "Obs send failed";
   }

   SMTP -> Disconnect();

   if (obs_sent_ok)
   {
      StatusBar1 -> SimpleText = "obs sent ok";
      info = "observation completed, you can quit TurboWin";
      ::MessageBox(0,info.c_str(), "TurboWin", MB_OK | MB_ICONASTERISK);
      Close();
   }
}
//---------------------------------------------------------------------------


