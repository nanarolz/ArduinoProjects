Dim Tempo
Dim erro_ant As Double, soma As Double
'Dim Kc As Double, tau_i As Double, tau_d As Double
Dim Kcp As Double
Dim Kc1 As Double, tau_i As Double
Dim contador_condensador As Integer
Dim cont As Integer
' ----------------------------------------------------------------------------- ABRIR PORTA COM
Sub Botão1_Clique()
    Dim x
    cont = 0
    contador_condensador = 2
    'x = Shell("C:\Users\LEAFT\Desktop\Reator\Teste_serial.exe", 1)
    ReturnValue = Shell("mode.com COM3 baud=9600 parity=n data=8 stop=1 to=off xon=off dtr=off rts=off")
    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 2)
    Open "COM3" For Binary Access Read Write As #1 'Open the com port
    Cells(1, 2).Value = "ON"
    Worksheets("Dados Experimentais").Cells(1, 1).Value = False
End Sub
' ----------------------------------------------------------------------------- FECHAR PORTA COM
Sub Botão2_Clique()
     Close #1
     Call StopTimer
     Cells(1, 2).Value = "OFF"
End Sub
' --------------------------------------------------------------------------- AQUISIÇÃO DE DADOS
Public Sub RunTimer()
    cont = cont + 1
    Tempo = Now
    Application.OnTime EarliestTime:=Tempo + TimeValue("00:00:05"), Procedure:="RunTimer", Schedule:=True
    erro_ant = 0
    soma = 0
    Kc = Worksheets("Dados Experimentais").Cells(3, 2).Value
    tau_i = Worksheets("Dados Experimentais").Cells(4, 2).Value
    tau_d = Worksheets("Dados Experimentais").Cells(5, 2).Value
    Call Timer1
    Call Timer2
End Sub
' --------------------------------------------------------------------------- TEMPERATURA 1
Sub Timer1()
        Dim SP As Double, medida As Double, erro As Double, R As Double, C As Double, char4 As String
        char1 = "e" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
        char3 = ""
             
        Do While Not char2 = " "
            char2 = Input(1, #1)
            char3 = char3 + char2
        Loop

        Worksheets("Dados Experimentais").Cells(cont, 3).Value = cont * 5
        Worksheets("Dados Experimentais").Cells(cont, 4).Value = char3
        Worksheets("Interface").Cells(14, 10).Value = char3
        
        'Valores dos parâmetros do controlador
        Kcp = 3.03
        Kc1 = 36.629
        tau_i = 4024.899
        
        If (Worksheets("Dados Experimentais").Cells(1, 1).Value = True) Then ' Resistência ON
            SP = Worksheets("Interface").Cells(12, 10).Value
            ' medida a ser controlada 14 - reator . 16 - banho
            medida = Worksheets("Interface").Cells(16, 10).Value
            erro = SP - medida
            soma = soma + (erro + erro_ant) * 5# / 2#
            'R = Kc * (erro + (1 / tau_i) * soma + tau_d * (erro - erro_ant) / 5)
            Worksheets("Dados Experimentais").Cells(14, 1).Value = contador_condensador
            contador_condensador = contador_condensador + 1
            
            If (erro > 0) Then
                
                R = Kcp * erro
                
                If (R < 0) Then ' 0%
                    R = 0
                End If
                
                If (R > 100) Then ' 100%
                    R = 100
                End If
                
                If (Int(R) = 100) Then
                    char1 = "R" + CStr(Int(R)) + Chr(13)
                Else
                    If (Int(R) > 10) Then
                        char1 = "R" + "0" + CStr(Int(R)) + Chr(13)
                    Else
                        char1 = "R" + "00" + CStr(Int(R)) + Chr(13)
                    End If
                End If
                Put #1, , char1
                Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
                
            Else

                char1 = "R" + "000" + Chr(13)
                Put #1, , char1
                Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
                
                C = Kc1 * (Abs(erro) + (1 / tau_i) * soma)
                
                If (C < 0) Then ' 0%
                    C = 0
                End If
                
                If (C > 100) Then ' 100%
                    C = 100
                End If
                
                If (contador_condensador > 24) Then
                    
                    contador_condensador = 0
                    
                    If (Int(C) = 100) Then
                        char1 = "j" + CStr(Int(C)) + Chr(13)
                    Else
                        If (Int(C) > 10) Then
                            char1 = "j" + "0" + CStr(Int(C)) + Chr(13)
                        Else
                            char1 = "j" + "00" + CStr(Int(C)) + Chr(13)
                        End If
                    End If
                    
                    Worksheets("Dados Experimentais").Cells(15, 1).Value = char1
                    Put #1, , char1
                    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
                
                End If
            End If
            
            'R = Worksheets("Interface").Cells(19, 10).Value
            erro_ant = erro
            
            Worksheets("Dados Experimentais").Cells(10, 1).Value = Int(R)
            Worksheets("Dados Experimentais").Cells(12, 1).Value = Int(C)
            
        End If
        
        If (Worksheets("Dados Experimentais").Cells(1, 1).Value = False) Then ' Resistência OFF
            char1 = "R" + "000" + Chr(13)
            Put #1, , char1
            char1 = "j" + "000" + Chr(13)
            Put #1, , char1
            Worksheets("Dados Experimentais").Cells(10, 1).Value = Int(R)
        End If
End Sub
' --------------------------------------------------------------------------- TEMPERATURA 2
Sub Timer2()
        char1 = "f" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
        char3 = ""
        
        Do While Not char2 = " "
            char2 = Input(1, #1)
            char3 = char3 + char2
        Loop

        Worksheets("Dados Experimentais").Cells(cont, 5).Value = char3
        Worksheets("Interface").Cells(16, 10).Value = char3
End Sub
' -------------------------------------------------------------------- PARAR AQUISIÇÃO DE DADOS
Public Sub StopTimer()
    On Error Resume Next
    Application.OnTime EarliestTime:=Tempo + TimeValue("00:00:05"), Procedure:="RunTimer", Schedule:=False

    ' zera a resistencia
    char1 = "R" + "000" + Chr(13)
    Put #1, , char1
    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)

    ' zera o condensador
    char1 = "j" + "000" + Chr(13)
    Put #1, , char1
    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
    
    Worksheets("Dados Experimentais").Cells(10, 1).Value = Int(R)
    Worksheets("Interface").Range("J14").ClearContents
    Worksheets("Interface").Range("J16").ClearContents
End Sub
' -------------------------------------------------------------------------------- LIMPAR DADOS
Sub Botão10_Clique()
    ' zera contador e planilha
    cont = 0
    Worksheets("Dados Experimentais").Range("C1:C3000").ClearContents
    Worksheets("Dados Experimentais").Range("D1:D3000").ClearContents
    Worksheets("Dados Experimentais").Range("E1:E3000").ClearContents
    Worksheets("Dados Experimentais").Range("J14").ClearContents
    Worksheets("Dados Experimentais").Range("J16").ClearContents
End Sub
