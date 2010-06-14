Public Class cpd
    Public Keys(0) As String
    Private astoi(0) As Integer
    Private isempty as Boolean = True

    Private aName As String
    Private aState As String
    Private aOrdConc(3) As Decimal
    Private afConc As Decimal
    Private asolThresh As Decimal
    Private aSharp As Decimal
    Private aRate(3) As Decimal

    Public Property Name() As String
        Get
            Name = aName
        End Get
        Set(ByVal value As String)
            aName = value
        End Set
    End Property
    Public Property State() As String
        Get
            State = aState
        End Get
        Set(ByVal value As String)
            aState = value
        End Set
    End Property
    Public ReadOnly Property Key() As String
        Get
            Key = aName & aState
        End Get
    End Property
    Public Property Conc(ByVal i As Integer) As Decimal
        Get
            Conc = aOrdConc(i - 1)
        End Get
        Set(ByVal value As Decimal)
            aOrdConc(i - 1) = value
        End Set
    End Property
    Public Property fConc() As Decimal
        Get
            fConc = afConc
        End Get
        Set(ByVal value As Decimal)
            afConc = value
        End Set
    End Property
    Public Property Rate(ByVal i As Integer) As Decimal
        Get
            Rate = aRate(i - 1)
        End Get
        Set(ByVal value As Decimal)
            aRate(i - 1) = value
        End Set
    End Property
    Public Property Thresh() As Decimal
        Get
            Thresh = asolThresh
        End Get
        Set(ByVal value As Decimal)
            asolThresh = value
        End Set
    End Property
    Public Property Sharp() As Decimal
        Get
            Sharp = aSharp
        End Get
        Set(ByVal value As Decimal)
            aSharp = value
        End Set
    End Property

    Public Sub AddStep(ByVal key As String, ByVal stoi As Integer)
        Dim i As Integer
        If isempty Then
            Keys(0) = key
            astoi(0) = stoi
            isempty = False
        Else
            For i = 0 To UBound(Keys)
                If Keys(i) = key Then
                    astoi(i) = astoi(i) + stoi
                    Exit Sub
                End If
            Next
            ReDim Preserve Keys(0 To UBound(Keys) + 1)
            ReDim Preserve astoi(0 To UBound(astoi) + 1)
            Keys(UBound(Keys)) = key
            astoi(UBound(astoi)) = stoi
        End If
    End Sub
    Public Property Stoi(ByVal key As String) As Integer
        Get
            Dim i As Integer
            For i = 0 To UBound(Keys)
                If key = Keys(i) Then
                    Stoi = astoi(i)
                    Exit Property
                End If
            Next
            Stoi = 0
        End Get
        Set(ByVal value As Integer)
            Dim i As Integer
            For i = 0 To UBound(Keys)
                If key = Keys(i) Then
                    astoi(i) = value
                    Exit Property
                End If
            Next
        End Set
    End Property
End Class