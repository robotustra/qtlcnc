#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QFile>
#include "layoutdata.h"
#include "path.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ggeom.setWidth(0);
    ggeom.setHeight(0);
    multiline_comment_on = FALSE;
    is_updating = FALSE;
    ld = NULL;
    ui->mainToolBar->hide();
    ui->menuBar->hide();
    socket = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if (socket != NULL){
        socket->disconnect();
    }
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
    QPainter painter(this);
    painter.fillRect(e->rect(), QColor(0,0,0,0));
    ld->draw_layout(painter, loffset);
}
// Mouse position handling for layout elements
void MainWindow::mousePressEvent(QMouseEvent *event){
    last_pos = event->pos();
    // look up the layout object.
    MyLayoutObject * obj = NULL;
    int o_type;
    QString cmd = QString();
    obj = ld->get_layout_object_at_mouse_pos(event->pos(), &o_type);
    if (obj != NULL){
        if (o_type == BUTTON){
            MyButton * mb = (MyButton*) obj;
            cmd = mb->get_active_state_command();

            ui->statusBar->showMessage( "Button detected at x="+ QString::number( event->x()) + " | y=" +QString::number( event->y()) + cmd );
        }
        //.. check other layout objects

    }else {
        ui->statusBar->showMessage( "x="+ QString::number( event->x()) + " | y=" +QString::number( event->y()) );
    }
    if (!cmd.isEmpty()) {
        ld->processCommand(cmd);
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    //int dx = event->x() - last_pos.x();
    //int dy = event->y() - last_pos.y();

    if (event->buttons() & Qt::LeftButton) {
        ;;
    } else if (event->buttons() & Qt::RightButton) {
        ;;
    }
    last_pos = event->pos();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    last_pos = e->pos();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    if(delta!=0)
    {
        ;;
    }
    last_pos = event->pos();
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
    LayoutData * ld = NULL;
    if (this->ld != NULL && this->ld->is_the_same_file(fname)) {
        qDebug() << "already have layout, reloading ";
        is_updating = true;
        ld = this->ld;

    }else{
        qDebug() << "new file";
        // loading layout first time
        if (this->ld != NULL) delete ld; //if we are here - the file is new
        is_updating = false;
        ld = new LayoutData(fname);
        this->ld = ld;
        qDebug() << "new layoutdata =" << ld;
    }
    qDebug() << "parsing = " << ld->is_parsing;
    if (ld->is_parsing){ return -1;}
    ld->is_parsing = true;
    // if the nile name of layout is different from this one - it should be reloaded
    // completely.
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
        qDebug()<< "size of list = " << stringList.size();
        QStringList list; //list of words to parse
        for (int i=0; i<stringList.size(); ++i)
        {
            QString str = stringList[i];
            if (str.isEmpty()) continue;
            QChar fs = QChar(str[0]);
            // Parsing file line by line.
            if (fs == QChar('#') ){
                continue; // skip comment line
            }
            else
            if (fs != QChar('\r') && fs != QChar('\n') ){
                // string to get,
                QString line = stringList[i].trimmed();

                if ( fs == QChar('\'') ){
                    // there is quoted text,
                    QString qstring = get_next_quoted_token(line);

                    while (qstring != NULL) {
                        list.push_back(qstring);
                        qstring = get_next_quoted_token(line);
                    }
                }else{
                    // does not need special spliting
                    //qDebug() << line;
                    QRegExp rx("[\t ]");// match tab or a space
                    //QString sline = line;
                    //qDebug() << "[" << sline << "]";
                    QStringList add_list = line.split(rx, QString::SkipEmptyParts);
                    //adding more vords to parse, support multiline input from file
                    for (int jj=0; jj<add_list.size(); jj++)
                        list.push_back(add_list[jj]);
                }
                ini_file_str += line + QString('\n');
                // now we have line we just have to parse it.
                okay = parse_list(list, ld);

            }
            if (!okay) {
                res = -1; break;
            }
        }
    }
    ld->is_parsing = false;
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
            // remove wotds from list[i] to the end of the list:
            while (i<list.size()) list.removeAt(i);
            return TRUE;
        } else
        if (!multiline_comment_on && is_string(ts)){
            //ts.remove("\"");
            //qDebug() << "type string:" << ts;
            cs++;
        }
        else if (!multiline_comment_on && is_word(ts))
        {
            //qDebug() << "this is a word, executing";
            qDebug() << "list before exec_word: "<< list;
            ok = exec_word(list, cs, ld);
            list.clear();
        }
        else if (!multiline_comment_on && is_type(ts))
        {
            qDebug() << "type modifier "<< ts;
            cs++;
        }
        else if (!multiline_comment_on && is_number(ts))
        {
            qDebug()<< "number found =" << ts << "\n";
            cs++;
        }
        else if (!multiline_comment_on && is_number_pair(ts))
        {
            qDebug()<< "number pair found =" << ts << "\n";
            cs++;
        }
        else if (!multiline_comment_on && is_var(ts))
        {
            qDebug()<< "var found =" << ts;
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
        else if (!multiline_comment_on && is_single_quote(ts)){
            // single quoted line
            //ts.remove("\'");
            cs++;
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
//Get the next token even if it's a quoted and cut it from the rest of the string.
QString MainWindow::get_next_quoted_token(QString& str){
    //qDebug() << "inside get_next_quoted_token " << str;
    if (str.isEmpty() || str.isNull()) return NULL;

    str.trimmed();
    while ( str[0] == QChar(' ') || str[0] == QChar('\t') ) str.remove(0,1);

    //qDebug() << "inside get_next_quoted_token " << str;
    QChar fs = QChar(str[0]);
    if (fs == QChar('\'') ){
        // has quotes, looking for the next quote
        int quote_offs = str.indexOf("' ", 1);
        if (quote_offs > 0){
            // have valid string (didn't check for quotes yet)
            //take substring from str
            QString lstr = str.left(quote_offs+1);
            //qDebug() << "[" << lstr << "]";
            //qDebug() << "{" << str << "}";
            str.remove(0,quote_offs+1);
            str.trimmed();
            return lstr;
        }
    } else {
        // look for the next space or tab of eol
        QString lstr = str.split(" ").at(0);
        lstr.trimmed();
        // cut the beginning of the string
        str.remove(0, lstr.size());
        //qDebug() << "inside lstr " << lstr;

        return lstr;
    }
    return NULL;
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
        "NEST",
        "IVAR",
        "IVEC2",
        "STRI",
        "PATH",
        "STATE",
        "BUTTON",
        "LAYOUT"

    };

    for (uint i=0; i< (sizeof(key_words)/sizeof(key_words[0])); i++){
        int x = QString::compare(s, QString::fromStdString( key_words[i] ), Qt::CaseInsensitive);
        if (x==0) return TRUE;
    }
    return FALSE;
}

// define the type of the data to interpret, these words are reserved and can't be used as variable.
bool MainWindow::is_type(QString& s){
std::string key_words[] = {
        "POS",
        "SIZE",
        "BGCOL",
        "PCOL",
        "PIX",
        "CMDL",
        "STATL"
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

bool MainWindow::is_single_quote(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    if ( tmps[0] == '\'' ){
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
    int comma_idx = s.indexOf(",");
    if ( (isdigit( tmps[0] )|| (((tmps[0] == '-' ) ||(tmps[0] == '+')) && isdigit(tmps[1]))) && comma_idx < 0 ){
        return TRUE;
    }
    return FALSE;
}

bool MainWindow::is_number_pair(QString& s){
    std::string tmps = s.toLocal8Bit().constData();
    int comma_idx = s.indexOf(",");
    if ( (isdigit( tmps[0] )|| (((tmps[0] == '-' ) ||(tmps[0] == '+')) && isdigit(tmps[1]))) && comma_idx >= 0 ){
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
    if(QString::compare( ts, "IVAR", Qt::CaseInsensitive) == 0){ // an integer number
        qDebug() << "IVAR found";
        //qDebug() << "before cs=" <<cs;
        ok = exec_IVAR(list, cs, ld);
        //qDebug() << "after cs=" <<cs;
    }
    if(QString::compare( ts, "IVEC2", Qt::CaseInsensitive) == 0){ // a int number 2d vector
        qDebug() << "IVEC2 found";
        ok = exec_IVEC2(list, cs, ld);
    }
    if(QString::compare( ts, "STRI", Qt::CaseInsensitive) == 0){ // string with spaces
        qDebug() << "STRI found";
        ok = exec_STRI(list, cs, ld);
    }
    if(QString::compare( ts, "PATH", Qt::CaseInsensitive) == 0){ // string with spaces
        qDebug() << "PATH found";
        ok = exec_PATH(list, cs, ld);
    }
    if(QString::compare( ts, "STATE", Qt::CaseInsensitive) == 0){ // state object
        qDebug() << "STATE found";
        ok = exec_STATE(list, cs, ld);
    }
    if(QString::compare( ts, "BUTTON", Qt::CaseInsensitive) == 0){ // button object
        qDebug() << "BUTTON found";
        ok = exec_BUTTON(list, cs, ld);
    }
    if(QString::compare( ts, "LAYOUT", Qt::CaseInsensitive) == 0){ // layout object
        qDebug() << "LAYOUT found";
        ok = exec_LAYOUT(list, cs, ld);
    }
    // some other words to add in this parser
    return ok; //FALSE by default
}

bool MainWindow::exec_IVAR(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "IVAR" keyword
    QString var = list.takeAt(cs); cs--;
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(INTN);
    }else{
        if (!is_updating) {
            qDebug() << "ivar " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            int fnum = get_int_value(list, cs, dl);
            dl->var_int_number[dl->val_index[v_idx]] = fnum;
            //qDebug() << "number is updated: " << fnum ;
            return TRUE;
        }
    }
    int fnum = get_int_value(list, cs, dl);
    dl->val_index.push_back(dl->var_int_number.size());
    dl->var_int_number.push_back(fnum); //file name
    dl->var_number_modified_flag.push_back(false); // variable is not modified.

    //qDebug() << "number is loaded :" << fnum ;
    return TRUE;
}

/* Path has a multiple string parameter to add.
 * Eash parameter has a mini command to encode the path:
 * 1) "10,20M" means  move(10,20)
 * 2) "30,40L" means  lineTo(30,40)
 * 3) "1,2,180,5A" means the arc with directing vector (1,2) CCW on 180 degree, with radius 5
 * 4) "1,2,-180,10A" means the arc with direction vector (1,2) CW on 180 degree, with radius 10.
 * 5) "1,2,30,40E" means closed circle or ellipse on the rectangle box
 * 6) "1,2,30,40R" means rectangle
 * 7) "1,2,4C"  means circle with the radius 4 and the center point (1,2)
 *
 * These are flat pathes, but for 3d stuff PATH3D should be use.
 *
 * All parameters of the PATH are strings except the name pf path.
 *
*/
bool MainWindow::exec_PATH(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "PATH" keyword
    QString var = list.takeAt(cs); cs--; // var name.
    // putting this var name to data structure
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(PATH);
    }else{
        if (!is_updating) {
            qDebug() << "path " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            Path2D* pa = get_path_value(list, cs, dl);
            dl->var_path[dl->val_index[v_idx]] = pa;
            //qDebug() << "path is updated: " << fnum ;
            return TRUE;
        }
    }
    Path2D* pa = get_path_value(list, cs, dl);
    dl->val_index.push_back(dl->var_path.size());
    dl->var_path.push_back(pa);
    dl->var_number_modified_flag.push_back(false); // new path
    //qDebug() << "path is loaded :" << fnum ;
    return TRUE;
}

bool MainWindow::exec_STATE(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "STATE" keyword
    QString var = list.takeAt(cs); cs--; // var name.
    // putting this var name to data structure
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(STATE);
    }else{
        if (!is_updating) {
            qDebug() << "state " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            MyState* ps = get_state_value(list, cs, dl);
            dl->var_state[dl->val_index[v_idx]] = ps;
            //qDebug() << "number is updated: " << fnum ;
            return TRUE;
        }
    }
    MyState* pa = get_state_value(list, cs, dl);
    dl->val_index.push_back(dl->var_state.size());
    dl->var_state.push_back(pa);
    dl->var_number_modified_flag.push_back(false); // new path
    //qDebug() << "state is loaded :" << fnum ;
    return TRUE;
}

bool MainWindow::exec_BUTTON(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "BUTTON" keyword
    QString var = list.takeAt(cs); cs--; // var name.
    // putting this var name to data structure
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(BUTTON);
    }else{
        if (!is_updating) {
            qDebug() << "button " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            MyButton * ps = get_button_value(list, cs, dl);
            dl->var_mybutton[dl->val_index[v_idx]] = ps;
            //qDebug() << "number is updated: " << fnum ;
            return TRUE;
        }
    }
    MyButton * pa = get_button_value(list, cs, dl);
    dl->val_index.push_back(dl->var_mybutton.size());
    dl->var_mybutton.push_back(pa);
    dl->var_number_modified_flag.push_back(false); // new layout
    //qDebug() << "state is loaded :" << fnum ;
    return TRUE;
}


bool MainWindow::exec_LAYOUT(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "LAYOUT" keyword
    QString var = list.takeAt(cs); cs--; // var name.
    // putting this var name to data structure
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(LAYOUT);
    }else{
        if (!is_updating) {
            qDebug() << "layout " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            SimpleLayout * ps = get_slayout_value(list, cs, dl);
            dl->var_slayout[dl->val_index[v_idx]] = ps;
            //qDebug() << "number is updated: " << fnum ;
            return TRUE;
        }
    }
    SimpleLayout* pa = get_slayout_value(list, cs, dl);
    dl->val_index.push_back(dl->var_slayout.size());
    dl->var_slayout.push_back(pa);
    dl->var_number_modified_flag.push_back(false); // new layout
    //qDebug() << "state is loaded :" << fnum ;
    return TRUE;
}


/*
    Saving multiword string in single quotes.
*/
bool MainWindow::exec_STRI(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "STRI" keyword
    QString var = list.takeAt(cs); cs--;
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(STRI);
    }else{
        if (!is_updating) {
            qDebug() << "stri " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            QString fstr = get_stri_value(list, cs, dl);
            dl->var_string[dl->val_index[v_idx]] = fstr;
            //qDebug() << "stri is updated: " << fstr ;
            return TRUE;
        }
    }
    QString fstr = get_stri_value(list, cs, dl);
    dl->val_index.push_back(dl->var_string.size());
    dl->var_string.push_back(fstr); //file name
    dl->var_number_modified_flag.push_back(false);

    qDebug() << "srti is loaded :" << fstr ;
    return TRUE;
}


bool MainWindow::exec_IVEC2(QStringList& list, int& cs, LayoutData *dl){
    list.removeAt(cs); cs--; //remove "IVEC" keyword
    QString var = list.takeAt(cs); cs--; //variable name
    int v_idx = dl->is_var_exist(dl, var);
    if (-1 == v_idx ){
        dl->var_name.push_back(var);
        dl->var_type.push_back(IVEC2);
    }else{
        if (!is_updating) {
            qDebug() << "ivec2 " << var << " exists, fix input file.";
            return FALSE;
        }else{
            // just update the value in layout
            int px = get_int_value(list, cs, dl); // takes value from the beginig
            int py = get_int_value(list, cs, dl);
            dl->var_ivec2[dl->val_index[v_idx]] = new QPoint(px,py);
            return TRUE;
        }
    }
    int px = get_int_value(list, cs, dl); // takes value from the beginig
    int py = get_int_value(list, cs, dl);
    dl->val_index.push_back(dl->var_ivec2.size());
    dl->var_ivec2.push_back(new QPoint(px,py)); //file name
    qDebug() << "ivec2 is added " << dl->var_ivec2[dl->var_ivec2.size()-1] ;
    return TRUE;
}


float MainWindow::get_float_value(QStringList& list, int& cs, LayoutData *dl){
    QString sv = list.takeAt(0); cs--;
    bool ok = FALSE;
    float v = QString(sv).toFloat(&ok);
    if (ok == TRUE) return v;
    else {
        int val_idx=dl->is_var_exist(dl, sv);
        if ( val_idx >= 0 ){ // have variable
            if (dl->var_type[val_idx] == DUBL){
                //qDebug() << "got number" << ar->var_number[ar->val_index[val_idx]];
                return dl->var_number[dl->val_index[val_idx]];
            }
        }else{
            qDebug() << "error, variable " << sv << " does not exist, fix the config file";
            return 0.0;
        }
    }
    return 0.0;
}

int MainWindow::get_int_value(QStringList& list, int& cs, LayoutData *dl){
    QString sv = list.takeAt(0); cs--;
    bool ok = FALSE;
    int v = QString(sv).toInt(&ok);
    if (ok == TRUE) return v;
    else {
        int val_idx = dl->is_var_exist(dl, sv);
        if ( val_idx >= 0 ){ // have variable
            if (dl->var_type[val_idx] == INTN){
                //qDebug() << "got number" << ar->var_number[ar->val_index[val_idx]];
                return dl->var_int_number[dl->val_index[val_idx]];
            }
        }else{
            qDebug() << "error, variable " << sv << " does not exist, fix the config file";
            return 0;
        }
    }
    return 0;
}

QString MainWindow::get_stri_value(QStringList& list, int& cs, LayoutData *dl){
    dl = dl;
    QString sv = list.takeAt(0); cs--;
    return sv;
}

/*
    This function is an interface to get INT value from layout data by name
*/
int MainWindow::get_int_value_from_data(QString var){
    int res = -999999999; //very big negative number.
    //qDebug() << "ld =" << ld;
    if (ld != NULL){
        int v_idx = ld->is_var_exist(ld, var);
        //qDebug() << "v_idx =" << v_idx;
        if (-1 != v_idx ){
             res = ld->var_int_number[ld->val_index[v_idx]];
        }else{
            qDebug()<< "variable name do not exist";
            exit (-1);
        }
    }
    return res;
}

QString MainWindow::get_stri_value_from_data(QString var){
    if (ld != NULL){
        int v_idx = ld->is_var_exist(ld, var);
        if (-1 != v_idx && (ld->var_type[v_idx] == STRI)){
             QString res = ld->var_string[ld->val_index[v_idx]];
             return res;
        }else{
            qDebug()<< "variable name do not exist";
            exit (-1);
        }
    }
    return NULL;
}



// Getting strings parameters and prepare constructor for Path2D
Path2D* MainWindow::get_path_value(QStringList& list, int& cs, LayoutData *dl){
    // no verification yet.
    cs = cs; // not used yet
    dl = dl; // not used yet
    return new Path2D(list);
}

// Getting strings parameters and prepare constructor State
MyState* MainWindow::get_state_value(QStringList& list, int& cs, LayoutData *dl){
    cs = cs; //not used yet
    // no verification yet.
    return new MyState(list, dl);
}

// Getting strings parameters and prepare constructor State
MyButton* MainWindow::get_button_value(QStringList& list, int& cs, LayoutData *dl){
    cs = cs; //not used yet
    // no verification yet.
    return new MyButton(list, dl);
}


// Getting strings parameters and prepare constructor State
SimpleLayout* MainWindow::get_slayout_value(QStringList& list, int& cs, LayoutData *dl){
    cs = cs; //not used yet
    dl = dl;
    // no verification yet.
    return new SimpleLayout(list);
}

int MainWindow::connect_to_server(){
    // connect to network
    // 1) read server options from file, IP address and PORT
    QString server_name = get_stri_value_from_data(QString("server_name"));
    server_name.remove("\"");
    int port = get_int_value_from_data("port");
    qDebug() << "Connecting to " << server_name << " port =" << port;

    if (socket != NULL) {
        socket->disconnect();
        socket = NULL;
    }
    socket = new QTcpSocket(this);

    socket->connectToHost(server_name, port);
    //Connect signals and slots
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(ui->connectButton,SIGNAL(clicked()),this, SLOT(connectToServer()));
    //connect(ui->disconnectButton,SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_connection()));
    timer->start(2000);
    return 0;
}

void MainWindow::check_connection(){
    if(socket == NULL) return;
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if (!connected) {
        qDebug() << "Could not connect to server";
    }else {
        qDebug() << "Connected";
    }
}

//Read data from socket
void MainWindow::readData()
{
    QString readLine = socket->readLine();
    QString repl = readLine.right(5);
    //bool ping_flag = false;
    if (readLine.contains("PING")){
        socket->write("PONG ");
        socket->write(repl.toUtf8().constData());
        //ping_flag = true;
    }
    qDebug()<< readLine;

    if(socket->canReadLine()) readData();
}

// This function prepare the message to send to the lcncrsh via socket.
//.............remake this template
void MainWindow::layoutDataChanged(){

    QString file_name = "../alma_output.txt";

    //QString message = QString ("File changed: ") + file_name;
    //ui->textEdit->append(message);
    //update();
    bool connected = (socket->state() == QTcpSocket::ConnectedState);

    if (connected) {
        QFile file(file_name); // this is a name of a file text1.txt sent from main method
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //qDebug()<< "File is not exist yet, skipping\n";
            return ;
        }
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString line = in.readLine();

        QString reply = line + QString(" \r\n");
        //ui->textEdit->append(reply);

        socket->write("PRIVMSG #eblarus :");
        //socket->write("PRIVMSG #belarus :");
        socket->write(line.toUtf8().constData());
        socket->write(" \r\n");
        socket->flush();

        file.close();
        QFile file1(file_name);
        file1.remove();
    }
}

void MainWindow::disconnectFromServer()
{
    timer->stop();

    socket->write("QUIT Good bye \r\n");
    socket->flush();
    socket->disconnect();
}
