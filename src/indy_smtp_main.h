//---------------------------------------------------------------------------

#ifndef indy_smtp_mainH
#define indy_smtp_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <IdMessage.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TIdSMTP *SMTP;
        TMemo *MemoBody;
        TIdMessage *IdMsgSend;
        TStatusBar *StatusBar1;
        TButton *ButtonSend;
        TEdit *EditTo;
        TEdit *EditSubject;
        TLabel *LabelTo;
        TLabel *LabelSubject;
        TLabel *LabelBody;
        TButton *Cancel;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ButtonSendClick(TObject *Sender);
        void __fastcall OnStatus(TObject *axSender,
        const TIdStatus axStatus, const AnsiString asStatusText);
        void __fastcall SMTPDisconnected(TObject *Sender);
        void __fastcall CancelClick(TObject *Sender);
private:	// User declarations
        void Send_SMTP();
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
