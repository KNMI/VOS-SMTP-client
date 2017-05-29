object Form1: TForm1
  Left = 324
  Top = 387
  BorderStyle = bsDialog
  Caption = 'TurboWin E-mail module'
  ClientHeight = 210
  ClientWidth = 592
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object LabelTo: TLabel
    Left = 48
    Top = 11
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object LabelSubject: TLabel
    Left = 25
    Top = 41
    Width = 39
    Height = 13
    Caption = 'Subject:'
  end
  object LabelBody: TLabel
    Left = 41
    Top = 70
    Width = 22
    Height = 13
    Caption = 'Obs:'
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 192
    Width = 592
    Height = 18
    Panels = <>
    SimplePanel = True
  end
  object ButtonSend: TButton
    Left = 210
    Top = 164
    Width = 75
    Height = 24
    Caption = 'Send'
    ModalResult = 1
    TabOrder = 1
    OnClick = ButtonSendClick
  end
  object EditTo: TEdit
    Left = 67
    Top = 9
    Width = 494
    Height = 21
    TabStop = False
    Enabled = False
    TabOrder = 2
  end
  object EditSubject: TEdit
    Left = 67
    Top = 39
    Width = 494
    Height = 21
    TabStop = False
    Enabled = False
    TabOrder = 3
  end
  object MemoBody: TMemo
    Left = 67
    Top = 68
    Width = 494
    Height = 88
    TabStop = False
    Enabled = False
    Lines.Strings = (
      '')
    TabOrder = 4
    WordWrap = False
  end
  object Cancel: TButton
    Left = 310
    Top = 164
    Width = 75
    Height = 24
    Caption = 'Cancel'
    TabOrder = 5
    OnClick = CancelClick
  end
  object SMTP: TIdSMTP
    OnStatus = OnStatus
    OnDisconnected = SMTPDisconnected
    Left = 36
    Top = 160
  end
  object IdMsgSend: TIdMessage
    BccList = <>
    CCList = <>
    Recipients = <>
    ReplyTo = <>
    Left = 4
    Top = 160
  end
end
