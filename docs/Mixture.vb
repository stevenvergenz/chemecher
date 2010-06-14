Public Class Mixture
    Public ChemSet As Collection
    Public StepSet As Collection
    Private atime As Decimal

    Public Property Time() As Decimal
        Get
            Time = atime
        End Get
        Set(ByVal value As Decimal)
            atime = value
        End Set
    End Property

    Public Sub New()
        ChemSet = New Collection
        StepSet = New Collection
    End Sub
End Class
