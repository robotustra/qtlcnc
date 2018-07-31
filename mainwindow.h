#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                      Chord, Pie, Path, Text, Pixmap };
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QSize get_screen_size();
    void set_window_size(QSize s, QPoint pt);
    void drawMesh();
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent * e);
    
private:
    Ui::MainWindow *ui;

    // Screen size
    QSize screen_size;

    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;
};

#endif // MAINWINDOW_H
