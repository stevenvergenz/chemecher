#ifndef DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPainter>

class DragListWidget : public QTableWidget
{
public:
    DragListWidget(QWidget *parent=0);
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
private:
	QPoint startpos;
};

#endif // DRAGLISTWIDGET_H