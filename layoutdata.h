#ifndef LAYOUTDATA_H
#define LAYOUTDATA_H

#include <QString>
#include <QPoint>
#include <QRect>
#include <QMatrix4x4>
#include <QPainter>
//#include <QtOpenGL/QtOpenGL> not ready for GL yet

#include <vector>
#include "glmesh.h"
#include "backdrop.h"
#include "mesh.h"
#include "lcs.h"
#include "myindicator.h"
#include "simplelayout.h"
#include "path.h"
#include "myglview.h"
#include "gcodeedit.h"
#include "gcodeview.h"

/*
*   This layout data contains all objects are needed to build qtlcnc layouts.
*   MainWindow is parsing and filling the layout data with objects.
*   The objects are recognized by layout data are
*   1) buttons
*   2) text labels
*   3) linear indicatiors
*   4) gl view
*   5) knobs?
*   6) layouts
*   7) some other stuff.
*/

enum BASE_TYPES
{
    STL,
    DUBL,   // defined by VAR
    VEC,
    QUAT,
    LCS_TYPE,
    MAT,
    MAT4,
    INTN,    // defined as IVAR
    IVEC2,  // 2dim vector of 2 ints
    STRI,   // multi word string, containing spaces or tabs.
    PATH,   // the vector of points on a plain
    STATE,  // A structure which represents the single state of any layout object, may include a bunch of properties.
    BUTTON, // layout object
    LABEL,  // text lable
    LIND,   // linear indicator
    KNOB,   // knob
    LAYOUT,
    GLVIEW,
    GCODEVIEW,
    GCODEEDIT,
    MAX_TYPE_INDEX
};

class MyState;
class MyButton;

class LayoutData
{
public:
    LayoutData(QString filename);
    void print_layout();
    int is_var_exist(LayoutData* ar, QString var);
    void draw_layout(QPainter & painter, QPoint &loffset); // draw current layout according to the current state.
    bool is_the_same_file(QString fn);
    MyLayoutObject * get_layout_object_by_name(QString& obj_name, int *type);
    MyLayoutObject * get_layout_object_at_mouse_pos(QPoint pos, int * type);
    MyState * get_state_object_by_name(QString& obj_name);
    QPoint* get_pos_var_by_name(QString & pos_name);
    int get_int_value_by_name(QString& int_name);
    QString get_string_value_by_name(QString& str_name);
    void set_string_value_by_name(QString str_name, QString& value);
    Path2D * get_path_value_by_name(QString& str_name);
    void processCommand(QString & cmd);


protected:

    std::vector<QString> var_name;
    std::vector<int> var_type;

    std::vector<QMatrix4x4> var_trans; // Variable transformations
    std::vector<QVector3D> var_vec;
    std::vector<int> val_index; // this is an index of value depending on type of value. If the value type is String, it's stored in var_string
                                // if it's STL - in mesh_arr, MAT4 ->transforms. For STL file we have 2 values, one if the file name and 2 is the
                                // mesh, how to store it?
    std::vector<float> var_number;
    std::vector<bool> var_number_modified_flag; //doubles, floats
    std::vector<bool> var_int_number_modified_flag; // ints
    std::vector<Mesh*> var_mesh_arr; // meshes, loaded from file.
    std::vector<Mesh*> var_mesh_arr_copy; // meshes, loaded from file.
    std::vector<QString> var_string; // string variable
    std::vector<Path2D*>  var_path;   // path variables for pictures
    std::vector<LCS*> var_lcs;      // local coordinate system
    std::vector<QVector4D> var_quat;// quaternions
    std::vector<int> var_int_number;    // defined as IVAR
    std::vector<QPoint*> var_ivec2;  // 2dim vector of 2 ints
    std::vector<MyState*> var_state;  // A structure which represents the single state of any layout object, may include a bunch of properties.
    std::vector<MyButton*> var_mybutton; // layout object
    std::vector<MyButton*> var_label;    // text lable is saved in the class button
    std::vector<MyIndicator*> var_indicator;    // linear indicator
    std::vector<MyIndicator*> var_knob;   // knob
    std::vector<MyGLView*> var_glview;   // OpenGL
    std::vector<GCodeView*> var_gcodeview;
    std::vector<GCodeEdit*> var_gcodeedit;
    std::vector<SimpleLayout*> var_slayout; // one screen layout

    QMatrix4x4 view_matrix() const; // We have only one view matrix

    float scale;
    float zoom;
    float tilt;
    float yaw;

    float perspective;

    QPoint mouse_pos;
    QString status;
    QString filename;
    bool is_parsing;
    int current_layout;

    friend class MainWindow;
};

#endif // LAYOUTDATA_H
