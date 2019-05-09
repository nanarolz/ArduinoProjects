' ----------------------------------------------------------------------------- BOMBA DO REATOR
Private Sub ToggleButton1_Click()
 Dim char1 As String, char2 As String
 If ToggleButton1.Value = True Then
        char1 = "a" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
'        If (char2 = "a") Then
'            Cells(2, 5).Value = "Bomba Reator ON"
'        Else
'            MsgBox ("Erro ao ligar a bomba do reator")
'        End If
    Else
        char1 = "b" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
'        If (char2 = "b") Then
'            Cells(2, 5).Value = "Bomba Reator OFF"
'        Else
'            MsgBox ("Erro ao desligar a bomba do reator")
'        End If
    End If
End Sub
' ------------------------------------------------------------------------ BOMBA DE RECIRCULAÇÃO
Private Sub ToggleButton2_Click()
 Dim char1 As String, char2 As String
 If ToggleButton2.Value = True Then
        char1 = "c" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
'        If (char2 = "c") Then
'            Cells(2, 7).Value = "Bomba Trocador ON"
'        Else
'            MsgBox ("Erro ao ligar a bomba recirculação")
'        End If
    Else
        char1 = "d" + Chr(13)
        Put #1, , char1  'write string to interface
        Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
'        If (char2 = "d") Then
'            Cells(2, 7).Value = "Bomba Trocador OFF"
'        Else
'            MsgBox ("Erro ao desligar a bomba recirculação")
'        End If
    End If
End Sub
' --------------------------------------------------------------------------- AQUISIÇÃO DE DADOS
Private Sub ToggleButton3_Click()
 If ToggleButton3.Value = True Then
    Call RunTimer
 Else
    Call StopTimer
 End If
End Sub
' ----------------------------------------------------------------------------------- AUTOMÁTICO
Private Sub ToggleButton4_Click()
If ToggleButton4.Value = True Then
    Worksheets("Dados Experimentais").Cells(1, 1).Value = True
Else
    Worksheets("Dados Experimentais").Cells(1, 1).Value = False
End If
End Sub
' ----------------------------------------------------------------------------------- ATOMIZADOR
Private Sub ToggleButton5_Click()
Dim char1 As String
If ToggleButton5.Value = True Then
    char1 = "l" + Chr(13)
    Put #1, , char1  'write string to interface
    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
Else
    char1 = "m" + Chr(13)
    Put #1, , char1  'write string to interface
    Application.Wait TimeSerial(Hour(Now()), Minute(Now()), Second(Now()) + 1)
End If
End Sub
