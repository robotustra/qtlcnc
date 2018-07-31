#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ggeom.setWidth(0);
    ggeom.setHeight(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSize MainWindow::get_screen_size(){
    QPixmap pm = QPixmap::grabWindow( QApplication::desktop()->winId());
    return QSize(pm.width(), pm.height());
}

void MainWindow::set_window_size(QSize s, QPoint pt){
    this->resize(s);
    this->move(pt);
    screen_size = QSize(s.width(),s.height()+pt.y()); //save size for future.
}

void MainWindow::paintEvent(QPaintEvent * e){
    e = e;
    static const QPoint points[4] = {
        QPoint(10, 10),
        QPoint(90, 10),
        QPoint(90, 90),
        QPoint(10, 90)
    };
    // example
    //painter.drawRect(0, 0, 100,100);
    //QRect rect(10, 20, 80, 60);

    QRect rect(0, 0, ucell-4, ucell-4);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QPainter painter(this);
    //pen = QPen("darkgreen");
    pen = QPen(QColor("black"), 1.5);

    painter.setPen(pen);

    QLinearGradient lg(0,0,100,100);

    //brush = QBrush("gray", Qt::LinearGradientPattern);
    brush = QBrush(Qt::NoBrush);
    painter.setBrush(brush);

    shape = Rect;
    painter.setRenderHint(QPainter::Antialiasing, true);

    transformed = false;
    for (int x = 0; x < ggeom.width() * ucell; x += ucell) {
        for (int y = 0; y < ggeom.height() * ucell; y += ucell) {
            painter.save();
            painter.translate(loffset.x() + x, loffset.y() + y);
            /*
            if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }*/

            switch (shape) {
            case Line:
                painter.drawLine(rect.bottomLeft(), rect.topRight());
                break;
            case Points:
                painter.drawPoints(points, 4);
                break;
            case Polyline:
                painter.drawPolyline(points, 4);
                break;
            case Polygon:
                painter.drawPolygon(points, 4);
                break;
            case Rect:
                painter.drawRect(rect);
                break;
            case RoundedRect:
                painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
                break;
            case Ellipse:
                painter.drawEllipse(rect);
                break;
            case Arc:
                painter.drawArc(rect, startAngle, arcLength);
                break;
            case Chord:
                painter.drawChord(rect, startAngle, arcLength);
                break;
            case Pie:
                painter.drawPie(rect, startAngle, arcLength);
                break;
            case Path:
                painter.drawPath(path);
                break;
            case Text:
                painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));
                break;
            case Pixmap:
                painter.drawPixmap(10, 10, pixmap);
            }
            painter.restore();
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

}

void MainWindow::set_ggeom(int w, int h){
    ggeom.setWidth(w);
    ggeom.setHeight(h);
}

void MainWindow::set_layout_params(QPoint p, int uc){
    loffset = p;
    ucell = uc;
}

int MainWindow::load_config(QString fname){
    int res = -1;
    if ( !fname.isEmpty() ){
        qDebug()<< fname;
        // parsing loayout configuration
        QFile file(fname);
        if (!file.open(QIODevice::ReadOnly))
        {
            perror("Can't open ini file to read");
            return -1;
        }
        ini_file_str.clear();
        bool okay = true;
        // read all file
        QStringList stringList;
        stringList.clear();
        QTextStream textStream(&file);
        while (true)
        {
            QString line0 = textStream.readLine();
            qDebug()<< line0;
            if (line0.isNull())
                break;
            else
                stringList.append(line0);
        }
        file.close();
        res = 0;
        for (int i=0; i<stringList.size(); ++i)
        {
            QString str = stringList[i];
            if (str.isEmpty()) continue;
            auto fs = str[0];
            // Parsing file line by line.
            if (fs == "#"){
               continue; // skip comment line
            }
            else
            if (fs != "\r" && fs != "\n"){
                // string to get,
                QString line = stringList[i].trimmed();
                //qDebug() << line;
                QRegExp rx("[\t ]");// match tab or a space
                //QString sline = line;
                //qDebug() << "[" << sline << "]";
                QStringList list = line.split(rx, QString::SkipEmptyParts);
                //qDebug() << list;
                ini_file_str += line + QString('\n');
                // now we have line we just have to parse it.
                okay = parse_list(list, ld);
                if (!okay) {
                    res = -1; break;
                }
            }
        }
    }
    return res;
}

/*
    The parser recognizes
    1) Comments
    2) Numbers
    3) variable names
    4) Words or operators, at the end of the line
    For simplicity one line - one operator.
*/
bool MainWindow::parse_list(QStringList& list, LayoutData* ld){
    int cs = 0; //position
    //qDebug() << list;
    // put string in the stack,
    // put variable in the stack
    // execute command
    bool ok = TRUE;
    int i =0;
    while (i<list.size()){
        QString ts = list[i];
        //qDebug() << ts;
        if (!multiline_comment_on && is_sl_comment(ts)){
            //qDebug()<< "single line comment";
            list.clear();
            return TRUE;
        } else
        if (!multiline_comment_on && is_string(ts)){
            ts.remove("\"");
            //qDebug() << "type string:" << ts;
            cs++;
        }
        else if (!multiline_comment_on && is_word(ts))
        {
            //qDebug() << "this is a word, executing";
            ok = exec_word(list, cs, ld);
            list.clear();
        }
        else if (!multiline_comment_on && is_number(ts))
        {
            cs++;
        }
        else if (!multiline_comment_on && is_var(ts))
        {
            cs++;
        }
        else if (is_ml_comment_start(ts)){
            multiline_comment_on = TRUE;
            //qDebug()<< "multiline comment starts";
        }
        else if (is_ml_comment_stop(ts)){
            multiline_comment_on = FALSE;
            //qDebug()<< "multiline comment ends";
        }
        else if (!multiline_comment_on && is_unknown(ts)){
            // parsing error, or # for comment. stop parsing
            qDebug()<< "error or comment symbol";
            return FALSE;
        }
        i++;
    }
    return ok;
}
bool MainWindow::is_word(QString& s){
std::string key_words[] = {
        "STL",
        "COPY",
        "VEC",
        "QUAT",
        "LCS",
        "MAT",
        "MAT4",
        "MUL",
        "MATT",
        "MATR",
        "VAR",
        "EQ",
        "MUL",
        "ADD",
        "SETO",
        "HIDE",
        "SHOW",
        "NEST"
    };

    for (uint i=0; i< (sizeof(key_words)/sizeof(key_words[0])); i++){
        int x = QString::compare(s, QString::fromStdString( key_words[i] ), Qt::CaseInsensitive);
        if (x==0) return TRUE;
    }
    return FALSE;
}


bool MainWindow::is_string(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( tmps[0] == '\"' ){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_ml_comment_start(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( tmps[0] == '/' && tmps[1] == '*'){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_ml_comment_stop(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( tmps[0] == '*' && tmps[1] == '/'){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_number(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( isdigit( tmps[0] )|| (((tmps[0] == '-' ) ||(tmps[0] == '+')) && isdigit(tmps[1])) ){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_var(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if (isalpha( tmps[0] )){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_unknown(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( !(isalpha(tmps[0])) ){
        return TRUE;
    }
    return FALSE;
}
bool MainWindow::is_sl_comment(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( tmps[0] == '#' ){
        return TRUE;
    }
    return FALSE;
}
/*
    Executing the word parsed.
*/
bool MainWindow::exec_word(QStringList& list, int& cs, LayoutData *ld){
    // cs points to the word to execute
    QString ts = list[cs];
    bool ok = FALSE;
    //qDebug() << "Executing word: " << ts;
    if (ld == NULL) {
        qDebug() << "empty data, skipping";
        return FALSE;
    }
    ok = TRUE; // for a while

    /*
    // We have only subset of interpreter, therefore  just execute it
    if(QString::compare( ts, "STL", Qt::CaseInsensitive) == 0){
        //qDebug() << "STL keyword"; // synax: "file.stl" stl_var_name STL
        ok = add_STL_var(list, cs, ar);
        return ok;
    }
    if(QString::compare( ts, "COPY", Qt::CaseInsensitive) == 0){
        ok = copy_STL_var(list, cs, ar);
        return ok;
    }
    if(QString::compare( ts, "LCS", Qt::CaseInsensitive) == 0){ // Local coordinate system
        // Coordinate system contains quaternion for rotation and an origin.
        //qDebug() << "LCS found"; // Syntax: vec_var quat_var lcs_name LCS
        //qDebug() << "before cs=" <<cs;
        ok = add_LCS_var(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "MUL", Qt::CaseInsensitive) == 0){
        //qDebug() << "MUL found";
        // matrix multiplication
        return ok;
    }
    if(QString::compare( ts, "QUAT", Qt::CaseInsensitive) == 0){ // Quaternion
        //qDebug() << "QUAT found";
        //qDebug() << "before cs=" <<cs;
        ok = add_QUAT_var(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "VEC", Qt::CaseInsensitive) == 0){ // 3D vector, or point
        //qDebug() << "VEC found";
        //qDebug() << "before cs=" <<cs;
        ok = add_VEC_var(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "SETO", Qt::CaseInsensitive) == 0){ // 3D vector, or point
        //qDebug() << "SETO found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_SETO(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "VAR", Qt::CaseInsensitive) == 0){ // a number
        //qDebug() << "VAR found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_VAR(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "NEST", Qt::CaseInsensitive) == 0){
        //qDebug() << "NEST found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_NEST(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "HIDE", Qt::CaseInsensitive) == 0){
        //qDebug() << "HIDE found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_HIDE(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }
    if(QString::compare( ts, "SHOW", Qt::CaseInsensitive) == 0){
        //qDebug() << "SHOW found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_SHOW(list, cs, ar);
        //qDebug() << "after cs=" <<cs;
        return ok;
    }*/
    // some other words to add in this parser
    return ok; //FALSE by default
}
