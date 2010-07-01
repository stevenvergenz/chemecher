#include "draglistwidget.h"

DragListWidget::DragListWidget(QWidget *parent) : QTableWidget(parent)
{
	setSelectionMode( QAbstractItemView::SingleSelection );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setGridStyle( Qt::DotLine );
	setWordWrap( false );
	setCornerButtonEnabled( false );
	setColumnCount( 2 );
	horizontalHeader()->setStretchLastSection( true );
	verticalHeader()->setVisible( false );
	verticalHeader()->setDefaultSectionSize( 20 );
	
	setHorizontalHeaderItem( 0, new QTableWidgetItem("Short Name") );
	setHorizontalHeaderItem( 1, new QTableWidgetItem("Concentration") );
	
	setMinimumSize(0,0);
	setMaximumSize(10000,10000);
}

void DragListWidget::mousePressEvent(QMouseEvent *event)
{
	if( event->button() == Qt::LeftButton && itemAt(event->pos())!=0
		&& itemAt(event->pos())->column()==0 )
		startpos = event->pos();
	else
		startpos = QPoint(-1,0);
	QTableWidget::mousePressEvent(event);
}

void DragListWidget::mouseMoveEvent(QMouseEvent *event)
{
	if( !(event->buttons() & Qt::LeftButton) || startpos.x() < 0 )
		return;	
	if( (event->pos() - startpos).manhattanLength() > QApplication::startDragDistance() ) {
		QMimeData *mimedata = new QMimeData();
		mimedata->setText(itemAt(startpos)->text());
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimedata);

		////
		
		// set vars
		QString text = itemAt(startpos)->text();
		QFont font = QFont("Sans", 10, 1, false);
		
		// prepare the pixmap for the drag
		QSize size = QFontMetrics(font).size(0, text);
		QImage image(size.width(), size.height(),
					 QImage::Format_ARGB32_Premultiplied);
		image.fill(qRgba(0, 0, 0, 0));   
		QPainter painter;
		painter.begin(&image);
		painter.setRenderHint(QPainter::Antialiasing);   
		painter.setFont(font);
		painter.setBrush(Qt::black);
		painter.drawText(QRect( QPoint(0,0), size), Qt::AlignCenter, text);
		painter.end();
		
		
		drag->setPixmap(QPixmap::fromImage(image));
		
		drag->setHotSpot(QPoint(size.width(), size.height()));
		drag->setDragCursor(QCursor(Qt::ArrowCursor).pixmap(), Qt::MoveAction);
		////
		
		drag->exec(Qt::MoveAction);
	}
	else {
		event->ignore();
	}
	//QListWidget::mouseMoveEvent(event);
}
