'CheMecher 3.2008.08.11, written by Steven Vergenz for Dr. Vergenz, University of North Florida
'New to 2008.08.11: fixed time scaling at higher orders
'New to 3.01 : reporting step works correctly, exit menu option added

Public Class frmMain
    Dim Mix As Mixture, Mixpr As Mixture
    Dim mechName As String, instName As String, outName As String, logName As String
    Dim CalcStep As Decimal, MaxTime As Integer, RptStep As Decimal, Transition As Integer
    Dim GateBand As Decimal, AutoStep As Boolean, ShiftTest As Integer, MaxReduce As Integer
    Dim StepFactor As Single, sOrder As String, Method As String, MaxOrder As Integer
    Dim Cancel As Boolean = False, ovrflo As Boolean = False

    Private Sub nuBeginSim_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuBeginSim.Click
        Dim fso As Scripting.FileSystemObject : fso = New Scripting.FileSystemObject
        Dim outFile As Scripting.TextStream, logFile As Scripting.TextStream
        Dim achm As cpd, bchm As cpd, astep As RxnStep
        Dim dTime As Decimal, Expon As Integer, ShiftCnt As Integer, RptCnt As Decimal = 0, Order As Integer
        Dim ra As Decimal, key As String, hBal As Decimal, Order2 As Integer, ca As Decimal
        Dim debugin As String, debuglst() As String

        outFile = fso.OpenTextFile(outName, Scripting.IOMode.ForWriting, True, Scripting.Tristate.TristateUseDefault)
        logFile = fso.OpenTextFile(logName, Scripting.IOMode.ForAppending, True, Scripting.Tristate.TristateUseDefault)

        'set up debug lines
        txtDebug.Enabled = False
        debuglst = txtDebug.Text.Split(",")
        For Each debugin In debuglst
            debugin.Trim()
        Next
        'txtDebug.Text = UBound(debuglst)

        'print header data to log
        logFile.WriteBlankLines(1)
        PrintData(logFile)

        'print output column headers
        outFile.Write("Time:") : WriteBlanks(outFile, ColSize - 5)
        If Not txtDebug.Text = "" Then
            For Each debugin In debuglst
                achm = Mix.ChemSet.Item(debugin)

                outFile.Write(achm.Key)
                WriteBlanks(outFile, ColSize - achm.Key.Length)

                For Order2 = 1 To MaxOrder
                    outFile.Write(achm.Key & "(" & Order2 & ")")
                    WriteBlanks(outFile, ColSize - (achm.Key & "(" & Order2 & ")").Length)
                Next
            Next
        Else
            For Each achm In Mix.ChemSet
                outFile.Write(achm.Key)
                WriteBlanks(outFile, ColSize - achm.Key.Length)
            Next
        End If
        outFile.WriteLine()

        'print initial concentrations to outfile
        outFile.Write(Format(0, nuTimeForm.Text))
        WriteBlanks(outFile, ColSize - Format(0, nuTimeForm.Text).Length)
        If Not txtDebug.Text = "" Then
            For Each debugin In debuglst
                achm = Mix.ChemSet.Item(debugin)
                outFile.Write(Format(achm.fConc, nuConcForm.Text))
                WriteBlanks(outFile, ColSize - Format(0, nuConcForm.Text).Length)
                WriteBlanks(outFile, ColSize * MaxOrder)
            Next
        Else

            For Each achm In Mix.ChemSet
                outFile.Write(Format(achm.fConc, nuConcForm.Text))
                WriteBlanks(outFile, ColSize - Format(achm.fConc, nuConcForm.Text).Length)
            Next
        End If
        outFile.WriteLine()

        'set up the progress bar
        nuProgress.Maximum = MaxTime
        nuProgress.Minimum = 0

        Mixpr = New Mixture
        nuCancel.Enabled = True
        nuBeginSim.Enabled = False
        nuConcForm.Enabled = False
        nuTimeForm.Enabled = False

        '*********************************************************\
        '                  Main Processing Loop                   |
        '*********************************************************/
        Do 'time loop
            My.Application.DoEvents()

            'copy mix values to mixpr
            Mixpr.ChemSet = Mix.ChemSet
            Mixpr.Time = Mix.Time
DownStep:
            'autostepping mechanism
            If AutoStep Then
                dTime = CalcStep * StepFactor ^ (-Expon)
                ShiftCnt = ShiftCnt + 1
            Else
                dTime = CalcStep
            End If

            'initialize new chemset and copy values
            Mix.ChemSet = New Collection
            For Each bchm In Mixpr.ChemSet
                achm = New cpd
                achm.Name = bchm.Name
                achm.State = bchm.State
                achm.Thresh = bchm.Thresh
                achm.Sharp = bchm.Sharp
                achm.fConc = bchm.fConc
                For Each key In bchm.Keys
                    achm.AddStep(key, bchm.Stoi(key))
                Next
                Mix.ChemSet.Add(achm, achm.Key)
            Next

            'increment time
            Mix.Time = Mixpr.Time + dTime
            nuStatus.Text = "Processing..." & Format(Mix.Time, nuTimeForm.Text)

            For Order = 1 To MaxOrder
                'calculate the rates
                For Each astep In Mix.StepSet
                    'first calculate positive rate of step (reactants)
                    ra = astep.kPlus
                    For Each key In astep.Reactants.Contents
                        achm = Mix.ChemSet(key)
                        bchm = Mixpr.ChemSet(key)
                        Select Case achm.State
                            Case "(.)", "(aq)", "(g)"
                                If Order > 1 Then
                                    ra = ra * achm.Conc(Order - 1)
                                Else
                                    ra = ra * bchm.fConc
                                End If
                            Case "(*)", "(s)", "(l)"
                                'configure the mixing coefficient
                                Select Case Transition
                                    Case 1 'arctangent transition
                                        If bchm.fConc > achm.Thresh Then
                                            hBal = 1
                                        ElseIf bchm.fConc > 0 Then
                                            'hbal atan s-curve D:[0,1] R:[0,1]
                                            hBal = (((System.Math.Atan(achm.Sharp * (bchm.fConc / achm.Thresh - 0.5)) * (2 / pi) + 1) / 2) - _
                                            ((System.Math.Atan(achm.Sharp * (-0.5)) * (2 / pi) + 1) / 2)) / _
                                            (((System.Math.Atan(achm.Sharp * (0.5)) * (2 / pi) + 1) / 2) - _
                                            ((System.Math.Atan(achm.Sharp * (-0.5)) * (2 / pi) + 1) / 2))
                                        Else
                                            hBal = 0
                                        End If
                                    Case 2 'linear transition
                                        If bchm.fConc > achm.Thresh Then
                                            hBal = 1
                                        ElseIf bchm.fConc > 0 Then
                                            hBal = bchm.fConc / achm.Thresh
                                        Else
                                            hBal = 0
                                        End If
                                    Case 3 'no transition
                                        hBal = 1
                                End Select 'transition
                                'calc rate for heterogeneous
                                If Order > 1 Then
                                    ra = ra * (hBal + (1 - hBal) * achm.Conc(Order - 1))
                                Else
                                    ra = ra * (hBal + (1 - hBal) * bchm.fConc)
                                End If

                        End Select 'achm.state

                    Next 'reactant
                    astep.VPlus(Order) = ra

                    'then conditionally calculate the reverse step (products)
                    ra = astep.kMins
                    For Each key In astep.Products.Contents
                        achm = Mix.ChemSet(key)
                        bchm = Mixpr.ChemSet(key)
                        Select Case achm.State
                            Case "(.)", "(aq)", "(g)"
                                If Order > 1 Then
                                    ra = ra * achm.Conc(Order - 1)
                                Else
                                    ra = ra * bchm.fConc
                                End If
                            Case "(*)", "(s)", "(l)"
                                'configure the mixing coefficient
                                Select Case Transition
                                    Case 1 'arctangent transition
                                        If bchm.fConc > achm.Thresh Then
                                            hBal = 1
                                        ElseIf bchm.fConc > 0 Then
                                            'hbal atan s-curve D:[0,1] R:[0,1]
                                            hBal = (((System.Math.Atan(achm.Sharp * (bchm.fConc / achm.Thresh - 0.5)) * (2 / pi) + 1) / 2) - _
                                            ((System.Math.Atan(achm.Sharp * (-0.5)) * (2 / pi) + 1) / 2)) / _
                                            (((System.Math.Atan(achm.Sharp * (0.5)) * (2 / pi) + 1) / 2) - _
                                            ((System.Math.Atan(achm.Sharp * (-0.5)) * (2 / pi) + 1) / 2))
                                        Else
                                            hBal = 0
                                        End If
                                    Case 2 'linear transition
                                        If bchm.fConc > achm.Thresh Then
                                            hBal = 1
                                        ElseIf bchm.fConc > 0 Then
                                            hBal = bchm.fConc / achm.Thresh
                                        Else
                                            hBal = 0
                                        End If
                                    Case 3 'no transition
                                        hBal = 1
                                End Select 'transition
                                'calc rate for heterogeneous
                                If Order > 1 Then
                                    ra = ra * (hBal + (1 - hBal) * achm.Conc(Order - 1))
                                Else
                                    ra = ra * (hBal + (1 - hBal) * bchm.fConc)
                                End If

                        End Select 'achm.state

                    Next 'product
                    astep.VMins(Order) = ra

                Next 'astep

                'after the collection of rates, calc collection of concs
                For Each achm In Mix.ChemSet
                    ra = 0 : bchm = Mixpr.ChemSet(achm.Key)
                    'fill in arcnx deal here using ra
                    For Each key In achm.Keys
                        astep = Mix.StepSet(key)
                        ra = ra + achm.Stoi(astep.Name) * (astep.VPlus(Order) - astep.VMins(Order))
                    Next
                    achm.Rate(Order) = ra

                    ca = 0
                    If Order < MaxOrder Then
                        For Order2 = 1 To Order
                            ca = ca + a(Order + 1, Order2 - 1) * achm.Rate(Order2)
                        Next
                        ca = ca * dTime + bchm.fConc
                        achm.Conc(Order) = ca
                    End If
                    If ca > 10000000000.0# Then ovrflo = True
                    achm.fConc = achm.fConc + dTime * b(Order) * achm.Rate(Order)
                Next
            Next 'order

            'autostepping mechanism
            If AutoStep Then
                For Each achm In Mix.ChemSet
                    bchm = Mixpr.ChemSet(achm.Key)
                    'refine timestep
                    If (bchm.fConc < achm.Thresh Or achm.fConc < achm.Thresh) _
                    And System.Math.Abs(achm.fConc - bchm.fConc) > GateBand * achm.Thresh _
                    And Expon <= MaxReduce Then
                        Expon = Expon + 1
                        ShiftCnt = 0
                        GoTo DownStep
                    End If
                    'enlarge timestep
                    If Expon > 0 And ShiftCnt >= ShiftTest Then
                        Expon = Expon - 1
                        ShiftCnt = 0
                    End If
                Next
            End If

            'print the fconcs to the output file
            RptCnt = RptCnt + dTime
            If System.Math.Abs(RptCnt - RptStep) <= 0.000000001 Then
                outFile.Write(Format(Mix.Time, nuTimeForm.Text))
                WriteBlanks(outFile, ColSize - Format(Mix.Time, nuTimeForm.Text).Length)

                'check for debug prints, otherwise print normally
                If Not txtDebug.Text = "" Then

                    For Each debugin In debuglst
                        achm = Mix.ChemSet.Item(debugin)
                        outFile.Write(Format(achm.fConc, nuConcForm.Text))
                        WriteBlanks(outFile, ColSize - Format(achm.fConc, nuConcForm.Text).Length)
                        For Order2 = 1 To MaxOrder
                            outFile.Write(Format(achm.Conc(Order2), nuConcForm.Text))
                            WriteBlanks(outFile, ColSize - Format(achm.Conc(Order2), nuConcForm.Text).Length)
                        Next
                    Next
                Else
                    For Each achm In Mix.ChemSet
                        outFile.Write(Format(achm.fConc, nuConcForm.Text))
                        WriteBlanks(outFile, ColSize - Format(achm.fConc, nuConcForm.Text).Length)
                    Next
                End If
                outFile.WriteLine()

                RptCnt = 0
            End If

            'increment the progress bar by integers
            nuProgress.Value = Mix.Time \ 1

        Loop Until Mix.Time >= MaxTime Or Cancel Or ovrflo
        '*******************************************************************
        '                  End of Main Processor Loop
        '*******************************************************************

        outFile.WriteBlankLines(1)
        PrintData(outFile)
        outFile.Close()

        nuCancel.Enabled = False
        nuBeginSim.Enabled = True
        nuTimeForm.Enabled = True
        nuConcForm.Enabled = True
        txtDebug.Enabled = True
        nuProgress.Value = 0
        nuStatus.Text = "Simulation Completed"
    End Sub

    Private Sub nuLoadMech_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuLoadMech.Click
        Dim fso As Scripting.FileSystemObject : fso = New Scripting.FileSystemObject
        Dim mechfile As Scripting.TextStream
        Dim nSpecies As Integer, nSteps As Integer
        Dim i As Integer, j As Integer
        Dim achm As cpd, astep As RxnStep
        Dim fileline() As String, processline() As String
        Mix = New Mixture
        Mix.Time = 0

        'configure dialog box
        dlgOpen.InitialDirectory = Application.StartupPath & "\Mechs\"
        dlgOpen.Filter = "Text Files|*.txt|All Files|*.*"
        If dlgOpen.ShowDialog() = Windows.Forms.DialogResult.OK Then
            mechName = dlgOpen.FileName
        Else
            Exit Sub
        End If
        'open mech file
        mechfile = fso.OpenTextFile(mechName, Scripting.IOMode.ForReading, False, Scripting.Tristate.TristateUseDefault)

        'read number of species
        nSpecies = Val(GetLine(mechfile))

        'read contained species list
        For i = 1 To nSpecies
            fileline = GetLine(mechfile).Split(" ")
            achm = New cpd
            achm.Name = fileline(0)
            achm.State = fileline(1)
            achm.Thresh = Val(fileline(2))
            achm.Sharp = Val(fileline(3))
            achm.fConc = 0
            'add new specie to collection
            Mix.ChemSet.Add(achm, achm.Key)
        Next

        'read number of steps
        nSteps = Val(GetLine(mechfile))

        'read contained step list
        For i = 1 To nSteps
            fileline = GetLine(mechfile).Split(" ")
            astep = New RxnStep
            astep.Name = Str(i)
            astep.kPlus = Val(fileline(1))
            astep.kMins = Val(fileline(2))

            'divide step into products and reactants
            fileline = fileline(0).Split(">")

            'divide reactants into species and add them
            processline = fileline(0).Split("+")
            For j = 0 To UBound(processline)
                astep.Reactants.Add(processline(j))
                achm = Mix.ChemSet(processline(j))
                achm.AddStep(astep.Name, -1)
            Next

            'divide products into species and add them
            processline = fileline(1).Split("+")
            For j = 0 To UBound(processline)
                astep.Products.Add(processline(j))
                achm = Mix.ChemSet(processline(j))
                achm.AddStep(astep.Name, 1)
            Next

            'almost forgot! add new step to collection
            Mix.StepSet.Add(astep, astep.Name)
        Next

        mechfile.Close()
        DrawMech()
        nuLoadInst.Enabled = True
        nuStatus.Text = "Load Simulation"
    End Sub
    Private Sub nuLoadInst_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuLoadInst.Click
        Dim fso As Scripting.FileSystemObject : fso = New Scripting.FileSystemObject
        Dim instFile As Scripting.TextStream
        Dim i As Integer, testline() As String, ord As Integer, meth As Integer

        'configure the open dialog box
        If dlgOpen.ShowDialog() = Windows.Forms.DialogResult.OK Then
            instName = dlgOpen.FileName
        Else
            Exit Sub
        End If

        'open the file
        instFile = fso.OpenTextFile(instName, Scripting.IOMode.ForReading, False, Scripting.Tristate.TristateUseDefault)

        'read the instance information
        For i = 1 To 11
            testline = GetLine(instFile).Split("=")
            Select Case UCase(testline(0))
                Case "CALCSTEP"
                    CalcStep = Val(testline(1))
                Case "REPORTSTEP"
                    RptStep = Val(testline(1))
                Case "MAXTIME"
                    MaxTime = Val(testline(1))
                Case "ORDER"
                    If testline(1) < 5 And testline(1) > 0 Then
                        ord = Val(testline(1))
                        MaxOrder = ord
                    Else
                        MsgBox("Order number invalid. Aborting simulation read.", MsgBoxStyle.OkOnly, "CheMecher 3")
                        instFile.Close()
                        Exit Sub
                    End If
                Case "METHOD"
                    Select Case UCase(testline(1))
                        Case "EULER"
                            meth = 1
                        Case "MODIFIED EULER"
                            meth = 2
                        Case "HEUN"
                            meth = 3
                        Case "RALSTON"
                            meth = 4
                        Case "RUNGE-KUTTA"
                            meth = 5
                        Case "RUNGE"
                            meth = 6
                        Case "KUTTA"
                            meth = 7
                        Case "GILL"
                            meth = 8
                    End Select
                Case "TRANSITION"
                    Select Case UCase(testline(1))
                        Case "ATAN"
                            Transition = 1
                        Case "LINEAR"
                            Transition = 2
                        Case "NONE"
                            Transition = 3
                    End Select
                Case "GATEBAND"
                    GateBand = Val(testline(1))
                Case "AUTOSTEP"
                    If UCase(testline(1)) = "YES" Then
                        AutoStep = True
                    Else
                        AutoStep = False
                    End If
                Case "SHIFTTEST"
                    ShiftTest = Val(testline(1))
                Case "MAXREDUCE"
                    MaxReduce = Val(testline(1))
                Case "STEPFACTOR"
                    StepFactor = Val(testline(1))
                Case Else
                    MsgBox("Bad simulation file. Aborting simulation read.", MsgBoxStyle.OkOnly, "CheMecher 3")
                    instFile.Close()
                    Exit Sub
            End Select
        Next

        'set the order/method constants
        DeclareConstants(ord, meth)

        'read initial concentrations
        Do
            testline = GetLine(instFile).Split(" ")
            If Mix.ChemSet.Contains(testline(0)) Then
                Mix.ChemSet(testline(0)).fConc = testline(1)
            End If
        Loop Until instFile.AtEndOfStream

        'update the various status boxes
        DrawInst()
        DrawOutfile()
        nuStatus.Text = "Simulation Ready"
        nuBeginSim.Enabled = True
    End Sub

    Private Function GetLine(ByRef f As Scripting.TextStream) As String
        Dim line As String, processline() As String

        'read until there is something useful
        Do
            line = f.ReadLine
            'If f.AtEndOfStream Then
            '    GetLine = ""
            '    Exit Function
            'End If
        Loop Until Not (line.StartsWith("'") Or line = "")

        'remove comments
        If line.Contains("'") Then
            processline = line.Split("'")
            line = processline(0)
        End If

        'return data block
        GetLine = line.Trim()
    End Function
    Private Sub DrawMech()
        Dim formstring As String
        Dim astep As RxnStep
        Dim achm As String

        'start drawing to the listbox
        For Each astep In Mix.StepSet
            'build the step string
            'example: 1 A(.) + B(.) --> C(.) k+=15 k-=.15
            formstring = astep.Name & " "
            For Each achm In astep.Reactants.Contents
                formstring = formstring & achm & " + "
            Next
            formstring = formstring.TrimEnd(" +".ToCharArray) & " --> "
            For Each achm In astep.Products.Contents
                formstring = formstring & achm & " + "
            Next
            formstring = formstring.TrimEnd(" +".ToCharArray)
            formstring = formstring & " k+=" & astep.kPlus & " k-=" & astep.kMins

            'add the string to the listbox
            lstMech.Items.Add(formstring)
        Next

    End Sub
    Private Sub DrawInst()
        Dim achm As cpd, drawstring As String

        'update all the label controls
        lblCalcStep.Text = CalcStep
        lblMaxTime.Text = MaxTime
        lblRptStep.Text = RptStep
        lblOrder.Text = sOrder
        lblMethod.Text = Method
        lblGateBand.Text = GateBand
        Select Case Transition
            Case 1
                lblTransition.Text = "arctangent"
            Case 2
                lblTransition.Text = "linear"
            Case 3
                lblTransition.Text = "none"
        End Select
        If AutoStep Then lblAutostep.Text = "Yes" Else lblAutostep.Text = "No"
        lblStepFactor.Text = StepFactor
        lblShiftTest.Text = ShiftTest
        lblMaxReduce.Text = MaxReduce

        'update the concentration box
        For Each achm In Mix.ChemSet
            drawstring = achm.Key & "=" & achm.fConc & Chr(9) _
            & "Thresh=" & achm.Thresh & Chr(9) & "Sharp=" & achm.Sharp
            lstConcs.Items.Add(drawstring)
        Next
    End Sub
    Private Sub PrintData(ByVal file As Scripting.TextStream)
        Dim i As Integer

        'write simulation data to file
        file.WriteLine("Generated by CheMecher " & Version & " on " & Format(Date.Now, "MMMM d, yyyy"))
        file.WriteLine("Copyright 2005, 2008 to Dr. Robert Vergenz, James Vickers, Steven Vergenz, and Timothy Vergenz")
        file.WriteLine("Output:" & Chr(9) & outName)
        file.WriteLine("Mech:" & Chr(9) & mechName)
        file.WriteLine("Sim:" & Chr(9) & instName)

        file.Write(sOrder & ", " & Method & ",  Heterogeneous Transition=")
        If Transition = 1 Then
            file.Write("Arctangent")
        ElseIf Transition = 2 Then
            file.Write("Linear")
        Else
            file.Write("None")
        End If
        file.WriteLine()
        file.WriteLine("Calculation Step=" & CalcStep & ", Report Step=" & RptStep & ", Maximum Time=" & MaxTime)
        'print autostep deal
        file.WriteLine("Autostep = " & AutoStep)
        If AutoStep Then
            file.WriteLine("Autostep Factor=" & StepFactor & ", GateBand=" & GateBand)
            file.WriteLine("# Finer Step Points=" & ShiftTest & ", Max Step Reduction=" & MaxReduce)
        End If

        'write the mech to file
        For i = 0 To lstMech.Items.Count - 1
            file.WriteLine(lstMech.Items.Item(i))
        Next

        'print species data to file
        For i = 0 To lstConcs.Items.Count - 1
            file.WriteLine(lstConcs.Items.Item(i))
        Next
    End Sub
    Private Sub DrawOutfile()
        Dim fso As Scripting.FileSystemObject : fso = New Scripting.FileSystemObject
        Dim mechbreak As String, instbreak As String, processline() As String
        Dim i As Integer

        'extract mech and inst names from filepaths
        processline = mechName.Split("\")
        mechbreak = processline(UBound(processline)).TrimEnd("tx".ToCharArray)
        processline = instName.Split("\")
        instbreak = processline(UBound(processline)).TrimEnd("tx".ToCharArray)

        'form the outfile name and path, and the log data
        outName = Application.StartupPath & "\CheMechDat\" & mechbreak & _
        instbreak & Format(Date.Now, DateFormat) & ".txt"
        logName = Application.StartupPath & "\CheMechDat\CheMechLog.txt"

        'check if the file already exists
        'if so, add an 'a' or 'b', etc. to the filename
        i = 0
        Do
            i = i + 1
            If fso.FileExists(outName) Then
                outName = Application.StartupPath & "\CheMechDat\" & mechbreak & _
                instbreak & Format(Date.Now, DateFormat) & Chr(96 + i) & ".txt"
            End If
        Loop Until Not fso.FileExists(outName)
        processline = outName.Split("\")
        lblOutname.Text = processline(UBound(processline))
        fso = Nothing
    End Sub

    Private Sub nuOutName_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles nuOutName.Click
        nuOutName.SelectAll()
    End Sub
    Private Sub nuOutName_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles nuOutName.TextChanged
        lblOutname.Text = nuOutName.Text
    End Sub
    Private Sub nuEditMech_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuEditMech.Click
        If Not mechName = "" Then Shell("notepad " & mechName, AppWinStyle.NormalFocus)
    End Sub
    Private Sub nuEditInst_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuEditInst.Click
        If Not instName = "" Then Shell("notepad " & instName, AppWinStyle.NormalFocus)
    End Sub
    Private Sub nuConcForm_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles nuConcForm.TextChanged
        lblConcForm.Text = nuConcForm.Text
    End Sub
    Private Sub nuTimeForm_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles nuTimeForm.TextChanged
        lblTimeForm.Text = nuTimeForm.Text
    End Sub

    Private Sub DeclareConstants(ByVal ord As Integer, ByVal meth As Integer)
        Select Case ord
            Case 1
                b(1) = 1 : a(1, 0) = 1
                sOrder = "1st Order" : Method = "Euler's Method"
            Case 2
                sOrder = "2nd Order"
                Select Case meth
                    Case 2
                        b(1) = 0 : a(1, 0) = 1 : b(2) = 1 : a(2, 0) = 1 : a(2, 1) = 1 / 2
                        Method = "Modified Euler's Method"
                    Case 3
                        b(1) = 1 / 2 : a(1, 0) = 1 : b(2) = 1 / 2 : a(2, 0) = 1 : a(2, 1) = 1
                        Method = "Heun's Method"
                    Case 4
                        b(1) = 1 / 3 : a(1, 0) = 1 : b(2) = 2 / 3 : a(2, 0) = 1 : a(2, 1) = 3 / 4
                        Method = "Ralston's Method"
                End Select
            Case 3
                b(1) = 1 / 6 : a(1, 0) = 1 : b(2) = 2 / 3 : a(2, 0) = 1 : a(2, 1) = 1 / 2
                b(3) = 1 / 6 : a(3, 0) = 1 : a(3, 1) = -1 : a(3, 2) = 2
                sOrder = "3rd Order" : Method = "Runge-Kutta Method"
            Case 4
                sOrder = "4th Order"
                Select Case meth
                    Case 6
                        b(1) = 1 / 6 : b(2) = 1 / 3 : b(3) = 1 / 3 : b(4) = 1 / 6
                        a(1, 0) = 1
                        a(2, 0) = 1 : a(2, 1) = 1 / 2
                        a(3, 0) = 1 : a(3, 1) = 0 : a(3, 2) = 1 / 2
                        a(4, 0) = 1 : a(4, 1) = 0 : a(4, 2) = 0 : a(4, 3) = 1
                        Method = "Runge Method"
                    Case 7
                        b(1) = 1 / 8 : b(2) = 3 / 8 : b(3) = 3 / 8 : b(4) = 1 / 8
                        a(1, 0) = 1
                        a(2, 0) = 1 : a(2, 1) = 1 / 3
                        a(3, 0) = 1 : a(3, 1) = -1 / 3 : a(3, 2) = 1
                        a(4, 0) = 1 : a(4, 1) = 1 : a(4, 2) = -1 : a(4, 3) = 1
                        Method = "Kutta Method"
                    Case 8
                        b(1) = 1 / 6 : b(2) = (2 - System.Math.Sqrt(2)) / 6 : b(3) = (2 + System.Math.Sqrt(2)) / 6 : b(4) = 1 / 6
                        a(1, 0) = 1
                        a(2, 0) = 1 : a(2, 1) = 1 / 2
                        a(3, 0) = 1 : a(3, 1) = (-1 + System.Math.Sqrt(2)) / 2 : a(3, 2) = (2 - System.Math.Sqrt(2)) / 2
                        a(4, 0) = 1 : a(4, 1) = 0 : a(4, 2) = -System.Math.Sqrt(2) / 2 : a(4, 3) = (2 + System.Math.Sqrt(2)) / 2
                        Method = "Gill Method"
                End Select
        End Select
    End Sub
    Private Sub WriteBlanks(ByRef file As Scripting.TextStream, ByVal n As Integer)
        Dim i As Integer
        For i = 1 To n
            file.Write(" ")
        Next
    End Sub

    Private Sub nuClear_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuClear.Click
        Mix.ChemSet = New Collection
        Mix.StepSet = New Collection
        lstMech.Items.Clear()
        lstConcs.Items.Clear()
        lblCalcStep.Text = ""
        lblRptStep.Text = ""
        lblMaxTime.Text = ""
        lblOrder.Text = ""
        lblMethod.Text = ""
        lblTransition.Text = ""

        lblAutostep.Text = ""
        lblStepFactor.Text = ""
        lblGateBand.Text = ""
        lblShiftTest.Text = ""
        lblMaxReduce.Text = ""

        lblOutname.Text = ""
        nuLoadInst.Enabled = False
    End Sub
    Private Sub AboutToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AboutToolStripMenuItem.Click
        MsgBox("CheMecher 3.0 written by Steven Vergenz, " & _
        "based on the adequate CheMecher 2.5 by Dr. Robert Vergenz " & _
        "of the University of North Florida. Copyright pending.", _
        MsgBoxStyle.OkOnly, "CheMecher 3.0")

    End Sub
    Private Sub nuCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles nuCancel.Click
        Cancel = True
    End Sub

    Private Sub HowToUseToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HowToUseToolStripMenuItem.Click
        Dim i As Integer
        i = Shell("notepad readme.txt", AppWinStyle.NormalFocus, False)
    End Sub

    Private Sub ExitToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitToolStripMenuItem.Click
        End
    End Sub
End Class

