#include "common.h"
#include "cpgui.h"
#include "ui_cpgui.h"
#include "qpainter.h"
#include "qfont.h"

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (controller_neighbor==0){
        painter.fillRect(event->rect(), QColor(0,0,0,0));
    }
    else{
        painter.fillRect(event->rect(), QColor(255, 255, 128));

    }
    drawRFpath(painter);
}

/*
* Function parse RF path from file and draw it
*/
void MainWindow::drawRFpath(QPainter& painter){

    // 0) setup brush
    int yo = -ui->drawFrame->geometry().y() - 2 * ui->horizontalLayout->spacing() - ui->menuBar->height();
    int xo = -ui->drawFrame->geometry().x() - 2 * ui->horizontalLayout->spacing();
    painter.setWindow(xo, yo,width(),height());

    //1) Get drawing area. and find the center of this area.

    //2) Take rf objects one by one from the rf_obj[] and draw them

    // the center of the draw frame
    tx0 =0.5*(ui->drawFrame->width() - ui->horizontalLayout->spacing()/2);
    ty0 =0.5*(ui->drawFrame->height()- ui->horizontalLayout->spacing()/2);

    //3)
    // ui->drawFrame->setToolTip("");

    for(int i=0; i<rf_obj_count; i++){
        drawRFobject(painter, i);
    }
}

void MainWindow::drawRFobject(QPainter& painter, int idx){

//!

    painter.setRenderHint(QPainter::Antialiasing);


    penColor = QColor("black");
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    //painter.setBrush(Qt::NoBrush);


    struct rfobj_ * rfo = &rf_obj[idx]; // our object to draw
    // Set gradient
    QLinearGradient gradient(0, 0, 0, rfo->sy);
    gradient.setColorAt(0.0, fillColor1);

    // Change color of the drowing object
    if (rfo->alarm)
        fillColor2 = QColor("red");
    else
        if (rfo->not_active)
            fillColor2 = QColor("slategray");
    else
        if (rfo->pre_alarm)
            fillColor2 = QColor("gold");
    else
        fillColor2 = QColor("darkgreen");

    gradient.setColorAt(1.0, fillColor2);
    painter.setBrush(gradient);

    int tx = tx0 + rfo->tx;
    int ty = ty0 + rfo->ty;
    QPainterPath path;
    QPainterPath select_path;

    painter.translate(tx, ty);

    switch (rfo->type) {
    case RF_DRIVER:
        {
            drawDriver(path, rfo->left, rfo->name);

        }
        break;
    case RF_SOURCE:
        {
            drawRFsource(path, rfo->left);
        }
        break;
    case RF_DRV_SWITCH_2_1:
        {
            drawDrvSwitch2_1(path, rfo->left);
        }
        break;
    case RF_DRV_SWITCH_1_2:
        {
            drawDrvSwitch1_2(path, rfo->left);
        }break;
    case RF_DRV_SPLITTER_1_2:
        {
            drawDrvSplitter1_2(path, rfo->left);
        }break;
    case RF_SPLITTER1_2_SM:
        {
            drawSplitter1_2_sm(path, rfo->left, rfo->name);
        }
        break;

    case RF_SPLITTER1_2_L:
        {
            drawSplitter1_2_l(path, rfo->left, rfo->name);
        }
        break;
    case RF_SPLITTER1_2_EL:
        {
            drawSplitter1_2_el(path, rfo->left, rfo->name);
        }
        break;
    case RF_SPLITTER1_4:
        {
            drawSplitter1_4(path, rfo->left, rfo->name);
        }
        break;
    case RF_SPLITTER1_8:
        {
            drawSplitter1_8(path, rfo->left, rfo->name);
        }
        break;
    case RF_AMPLIFIER:
        {
            drawAmplifier(path, rfo->left, rfo->name);
        }
        break;
    case RF_COMBINER4_1:
        {
            drawCombiner4_1(path, rfo->left, rfo->name);
        }
        break;
    case RF_COMBINER2_1:
        {
            drawCombiner2_1(path,rfo->left, rfo->name);
        }
        break;
    case RF_COMBINER_CONTROLLER:
        {
            drawCombiner_Controller(path,rfo->left, rfo->name);
        }
        break;
    case RF_POWER_SUPPLY:
        {
            drawPower_Supply(path,rfo->left, rfo->name);
        }
        break;
    case RF_CIRCULATOR:
        {
            drawCirculator(path, rfo->left, rfo->name);
        }
        break;
    case RF_WG_SWITCH:
        {
            drawWGSwitch(path, rfo->left, rfo->name);
        }
        break;
    case RF_COMBINER2_1_H:
        {
            drawCombiner2_1_H(path, rfo->left, rfo->name);
        }
        break;
    case RF_LINE:
        {
            drawLine(path, rfo->sx, rfo->sy);
        }
        break;
    case RF_REV_PWR_DETECTOR:
        {
            drawRPWRDetectror(path, rfo->left, rfo->name, rfo->dvalue);
        }
        break;
    case RF_FWD_PWR_DETECTOR:
        {
            drawFPWRDetectror(path, rfo->left, rfo->name, rfo->dvalue);
        }
        break;
    case RF_ANTENA:
        {
            drawRFout(path, rfo->left, rfo->name);
        }
    default:
        break;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("lightgray"));
    painter.drawPath(path.translated(QPointF(5, 5)));

    painter.setPen(QColor("black"));
    painter.setBrush(gradient);
    painter.drawPath(path);

    if (rfo->selected){
        penColor = QColor("gray");
        painter.setPen(QPen(penColor, penWidth, Qt::DashLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.setBrush(Qt::NoBrush);
        drawSelectedPath(select_path,rfo->sx,rfo->sy);
        // ui->drawFrame->setToolTip(rfo->name);
        painter.drawPath(select_path);
    }
    painter.setPen(QColor("black"));

    painter.translate(-tx, -ty);

}
