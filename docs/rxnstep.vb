Public Class RxnStep
    Public Reactants As ColArray, Products As ColArray
    Private nReactant As Integer, nProduct As Integer
    Private aname As String
    Private akplus As Decimal, akmins As Decimal
    Private arplus(3) As Decimal, armins(3) As Decimal

    Public Property Name() As String
        Get
            Name = aname
        End Get
        Set(ByVal value As String)
            aname = value
        End Set
    End Property
    Public Property kPlus() As Decimal
        Get
            kPlus = akplus
        End Get
        Set(ByVal value As Decimal)
            akplus = value
        End Set
    End Property
    Public Property kMins() As Decimal
        Get
            kMins = akmins
        End Get
        Set(ByVal value As Decimal)
            akmins = value
        End Set
    End Property
    Public Property VPlus(ByVal i As Integer) As Decimal
        Get
            VPlus = arplus(i - 1)
        End Get
        Set(ByVal value As Decimal)
            arplus(i - 1) = value
        End Set
    End Property
    Public Property VMins(ByVal i As Integer) As Decimal
        Get
            VMins = armins(i - 1)
        End Get
        Set(ByVal value As Decimal)
            armins(i - 1) = value
        End Set
    End Property

    Public Sub New()
        Reactants = New ColArray
        Products = New ColArray
    End Sub
End Class

Public Class ColArray
    Public Contents(0) As String
    Private aCount As Integer
    Private isempty As Boolean = True

    Public Sub Add(ByVal key As String)
        If isempty Then
            Contents(0) = key
            isempty = False
        Else
            ReDim Preserve Contents(0 To UBound(Contents) + 1)
            Contents(UBound(Contents)) = key
        End If

        aCount = aCount + 1
    End Sub

    Public ReadOnly Property Count()
        Get
            Count = aCount
        End Get
    End Property
End Class