#ifndef LAYOUTDATA_H
#define LAYOUTDATA_H

#include <QString>
#include <QPoint>
#include <QRect>
#include "state.h"
//#include <QtOpenGL/QtOpenGL> not ready for GL yet

#include <vector>
//#include "glmesh.h"
//#include "backdrop.h"
//#include "mesh.h"
//#include "lcs.h"

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
    INT,    // defined as IVAR
    GEOM,   // application screen geometry
    IVEC2,  // 2dim vector of 2 ints
    STATE,  // A structure which represents the single state of any layout object, may include a bunch of properties.
    BUTTON, // layout object
    LABEL,  // text lable
    LIND,   // linear indicator
    KNOB,   // knob
    LAYOUT,
    MAX_TYPE_INDEX
};

class LayoutData
{
public:
    LayoutData(QString filename);
    void print_layout();
    int is_var_exist(LayoutData* ar, QString var);
    void draw_layout(); // draw current layout according to the current state.

protected:

    std::vector<QString> var_name;
    std::vector<int> var_type;

    std::vector<QMatrix4x4> var_trans; // Variable transformations
    std::vector<QVector3D> var_vec;
    std::vector<int> val_index; // this is an index of value depending on type of value. If the value type is String, it's stored in var_string
                                // if it's STL - in mesh_arr, MAT4 ->transforms. For STL file we have 2 values, one if the file name and 2 is the
                                // mesh, how to store it?
    std::vector<float> var_number;
    std::vector<bool> var_number_modified_flag;
    std::vector<Mesh*> var_mesh_arr; // meshes, loaded from file.
    std::vector<Mesh*> var_mesh_arr_copy; // meshes, loaded from file.
    std::vector<LCS*> var_lcs;      // local coordinate system
    std::vector<QVector4D> var_quat;// quaternions
    std::vector<int> var_int_number;    // defined as IVAR
    std::vector<QRect> var_geometry;    // application screen geometry
    std::vector<QPoint> var_ivec2;  // 2dim vector of 2 ints
    std::vector<State*> var_state;  // A structure which represents the single state of any layout object, may include a bunch of properties.
    std::vector<MyButton*> var_mybutton; // layout object
    LABEL,  // text lable
    LIND,   // linear indicator
    KNOB,   // knob
    LAYOUT,

    QMatrix4x4 view_matrix() const; // We have only one view matrix

    // Background
    Backdrop* backdrop;

    float scale;
    float zoom;
    float tilt;
    float yaw;

    float perspective;
    //Q_PROPERTY(float perspective WRITE set_perspective);
    QPropertyAnimation anim;

    QPoint mouse_pos;
    QString status;
    QString filename;
    QString file_content;
    bool is_parsing;
    bool is_transformed;

    friend class ArenaLoader;
    friend class Canvas;
    friend class PoseCapture;
    friend class PoseEditDlg;
    friend class Window;


};

#endif // LAYOUTDATA_H
