#include "layoutdata.h"

LayoutData::LayoutData(QString filename)
{
    fname = filename;
}


/*Display all content of arena*/
void LayoutData::print_layout(){
    qDebug() << "layout has Vars: " << var_name.size();
    //qDebug() << "Vectors: " << var_vec.size();
    //qDebug() << "Numbers: " << var_number.size();
    //qDebug() << "Meshes: " << var_mesh_arr.size();
    //qDebug() << "LCS: " << var_lcs.size();
    //qDebug() << "Quats: " << var_quat.size();

}

int LayoutData::is_var_exist(LayoutData* ld, QString var){
    int i=-1;
    for (uint j = 0; j<ld->var_name.size(); j++){
        int x = QString::compare(ld->var_name[j], var, Qt::CaseSensitive);
        if ( x == 0 ) {
            i = j;
            return i;
        }
    }
    return i;
}

void LayoutData::draw_layout(){

}
