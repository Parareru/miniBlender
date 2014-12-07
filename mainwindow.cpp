#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "attributewidget.h"
#include "enity.h"
#include "OBJ.h"
#include <QFileDialog>
#include <vector>
#include <QDebug>

using namespace std;
extern vector<model> models;
struct attribute{
    QString modelName;
    int modelIndex;
};
vector<attribute> objAttr;
vector<attribute> entityAttr;
int indexCounter;

//用于右键菜单
QTreeWidgetItem* curItem;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widget = new GLWidget();
    widget->setObjectName(QStringLiteral("opengl"));
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout_3->addWidget(widget);
    widget->setFocusPolicy(Qt::ClickFocus);

    attr = new attributeWidget();
    ui->verticalLayout_4->addWidget(attr);
    initialCatalog();
    indexCounter = 0;

    connect(widget, SIGNAL(model_select()), this, SLOT(initSpinBoxAndSlider()));
    connect(widget, SIGNAL(model_select()), this, SLOT(selectedAttribute()));
    connect(widget, SIGNAL(model_select()), this, SLOT(getSelectedItem()));
    connect(attr, SIGNAL(reshape()), this, SLOT(reshapeEntity()));
    connect(this, SIGNAL(objectSubmit(bool)), this, SLOT(updateCatalog(bool)));
    connect(this, SIGNAL(sendSelectOBJ(int)), widget, SLOT(modelSelect(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getSelectedItem()
{
    int id = widget->selectedID-1;
    if(id < 0)
        return;
    for(unsigned int i = 0; i< objAttr.size(); i++){
        if(objAttr[i].modelIndex == id){
            QTreeWidgetItem *parent = ui->treeWidget->topLevelItem(0);
            curItem = parent->child(i);
        }
    }
    for(unsigned int i = 0; i< entityAttr.size(); i++){
        if(entityAttr[i].modelIndex == id){
            QTreeWidgetItem *parent = ui->treeWidget->topLevelItem(1);
            curItem = parent->child(i);
        }
    }
//    qDebug()<<"curItem";
//    qDebug()<<curItem->text(0);
}

void MainWindow::on_actionImport_OBJ_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("open file"),
                                                    "E:/Study/ComputerGraphics/qtopengl/OBJ",
                                                    tr("OBJ file(*.obj)"));
    if(fileName != ""){
      widget->initOBJ();
      loadOBJ(fileName.toStdString().c_str());
      widget->updateGL();

      emit objectSubmit(false);
      }
}

void MainWindow::on_actionWire_Solid_triggered()
{
    if(widget->isWireframe){
        widget->isWireframe = false;
      }
    else{
        widget->isWireframe = true;
      }
}

void MainWindow::on_actionLight_On_Off_triggered()
{
    if(widget->enableLight){
        widget->enableLight = false;
      }
    else{
        widget->enableLight = true;
      }
}

void MainWindow::initSpinBoxAndSlider()
{
    ui->doubleSpinBox->setValue(0.0);
    ui->doubleSpinBox_2->setValue(0.0);
    ui->doubleSpinBox_3->setValue(0.0);
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->doubleSpinBox->setValue(value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->doubleSpinBox_2->setValue(value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->doubleSpinBox_3->setValue(value);
}

void MainWindow::on_doubleSpinBox_valueChanged(double rotate_x)
{
    if(widget->selectedID){
        if(ui->doubleSpinBox_2->text().toFloat() != 0.0){
            glPushMatrix();
            glLoadMatrixf(widget->lastRotateMatrix);
            glRotatef(ui->doubleSpinBox_2->text().toFloat(), 0, 1, 0);
            glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
            glPopMatrix();
            for(int i=0; i<16; i++)
              widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
            disconnect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_2_valueChanged(double)));
            ui->doubleSpinBox_2->setValue(0.0);
            ui->horizontalSlider_2->setValue(0);
            connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_2_valueChanged(double)));
          }
        if(ui->doubleSpinBox_3->text().toFloat() != 0.0){
            glPushMatrix();
            glLoadMatrixf(widget->lastRotateMatrix);
            glRotatef(ui->doubleSpinBox_3->text().toFloat(), 0, 0, 1);
            glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
            glPopMatrix();
            for(int i=0; i<16; i++)
              widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
            disconnect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_3_valueChanged(double)));
            ui->doubleSpinBox_3->setValue(0.0);
            ui->horizontalSlider_3->setValue(0);
            connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_3_valueChanged(double)));
          }
        glPushMatrix();
        glLoadMatrixf(widget->lastRotateMatrix);
        glRotatef(rotate_x, 1, 0, 0);
        glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
        glPopMatrix();
      }
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double rotate_y)
{
    if(widget->selectedID){
          if(ui->doubleSpinBox->text().toFloat() != 0.0){
              glPushMatrix();
              glLoadMatrixf(widget->lastRotateMatrix);
              glRotatef(ui->doubleSpinBox->text().toFloat(), 1, 0, 0);
              glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
              glPopMatrix();
              for(int i=0; i<16; i++)
                widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
              disconnect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_valueChanged(double)));
              ui->doubleSpinBox->setValue(0.0);
              ui->horizontalSlider->setValue(0);
              connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_valueChanged(double)));
            }
          if(ui->doubleSpinBox_3->text().toFloat() != 0.0){
              glPushMatrix();
              glLoadMatrixf(widget->lastRotateMatrix);
              glRotatef(ui->doubleSpinBox_3->text().toFloat(), 0, 0, 1);
              glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
              glPopMatrix();
              for(int i=0; i<16; i++)
                widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
              disconnect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_3_valueChanged(double)));
              ui->doubleSpinBox_3->setValue(0.0);
              ui->horizontalSlider_3->setValue(0);
              connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_3_valueChanged(double)));
            }
          glPushMatrix();
          glLoadMatrixf(widget->lastRotateMatrix);
          glRotatef(rotate_y, 0, 1, 0);
          glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
          glPopMatrix();
        }
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double rotate_z)
{
    if(widget->selectedID){
          if(ui->doubleSpinBox_2->text().toFloat() != 0.0){
              glPushMatrix();
              glLoadMatrixf(widget->lastRotateMatrix);
              glRotatef(ui->doubleSpinBox_2->text().toFloat(), 0, 1, 0);
              glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
              glPopMatrix();
              for(int i=0; i<16; i++)
                widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
              disconnect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_2_valueChanged(double)));
              ui->doubleSpinBox_2->setValue(0.0);
              ui->horizontalSlider_2->setValue(0);
              connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_2_valueChanged(double)));
            }
          if(ui->doubleSpinBox->text().toFloat() != 0.0){
              glPushMatrix();
              glLoadMatrixf(widget->lastRotateMatrix);
              glRotatef(ui->doubleSpinBox->text().toFloat(), 1, 0, 0);
              glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
              glPopMatrix();
              for(int i=0; i<16; i++)
                widget->lastRotateMatrix[i] = models[widget->selectedID-1].rotateMatrix[i];
              disconnect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_valueChanged(double)));
              ui->doubleSpinBox->setValue(0.0);
              ui->horizontalSlider->setValue(0);
              connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_valueChanged(double))
                      );
            }
          glPushMatrix();
          glLoadMatrixf(widget->lastRotateMatrix);
          glRotatef(rotate_z, 0, 0, 1);
          glGetFloatv(GL_MODELVIEW_MATRIX, models[widget->selectedID-1].rotateMatrix);
          glPopMatrix();
        }
}

void MainWindow::initialCatalog()
{
    ui->treeWidget->clear();
    QTreeWidgetItem *Objects = new QTreeWidgetItem(QStringList()<<"Objects");
    ui->treeWidget->addTopLevelItem(Objects);
    QTreeWidgetItem *Entities = new QTreeWidgetItem(QStringList()<<"Entities");
    ui->treeWidget->addTopLevelItem(Entities);
}

void MainWindow::updateCatalog(bool EntityOrObject)
{
    if(EntityOrObject == true){
        QTreeWidgetItem *topItem = ui->treeWidget->topLevelItem(1);
        QString name = "entity" + QString::number(entityAttr.size()+1);
        attribute ent;
        ent.modelName = name;
        ent.modelIndex = indexCounter;
        entityAttr.push_back(ent);
        QTreeWidgetItem *newObj = new QTreeWidgetItem(QStringList()<<name);
        topItem->addChild(newObj);

        indexCounter++;
    }
    else{
        QTreeWidgetItem *topItem = ui->treeWidget->topLevelItem(0);
        QString name = "object" + QString::number(objAttr.size()+1);
        attribute obj;
        obj.modelName = name;
        obj.modelIndex = indexCounter;
        objAttr.push_back(obj);
        QTreeWidgetItem *newObj = new QTreeWidgetItem(QStringList()<<name);
        topItem->addChild(newObj);

        indexCounter++;
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();
    if(parent == NULL)
        return;
    int index = parent->indexOfChild(item);
    int currentModelID;
    if(parent->text(0) == "Objects")
       currentModelID = objAttr[index].modelIndex;
    else
       currentModelID = entityAttr[index].modelIndex;
//    qDebug()<<currentModelID;

    emit sendSelectOBJ(currentModelID + 1);
}


void MainWindow::on_actionAdd_cube_triggered()
{
    model Cube = createCube();
    Cube.type = CUBE;
    models.push_back(Cube);

    emit objectSubmit(true);
}

void MainWindow::on_actionAdd_prism_triggered()
{
    Prismoid prismoid(3, 50, 50, 50);
    model P = prismoid.createPrismoid();
    P.type = PRISMOID;
    models.push_back(P);

    emit objectSubmit(true);
}

void MainWindow::on_actionAdd_prismoid_triggered()
{
    Prismoid prismoid;
    model P = prismoid.createPrismoid();
    P.type = PRISMOID;
    models.push_back(P);

    emit objectSubmit(true);
}

void MainWindow::on_actionAdd_sphere_triggered()
{
    Sphere sphere;
    model s = sphere.createSphere();
    s.type = SPHERE;
    models.push_back(s);

    emit objectSubmit(true);
}

void MainWindow::on_actionAdd_cylinder_triggered()
{
    Prismoid prismoid(50,50,50,50);
    model P = prismoid.createPrismoid();
    P.type = CYLINDER;
    models.push_back(P);

    emit objectSubmit(true);
}

void MainWindow::on_actionAdd_cone_triggered()
{
    Cone cone;
    model C = cone.createCone();
    C.type = CONE;
    models.push_back(C);

    emit objectSubmit(true);
}

void MainWindow::selectedAttribute()
{
    if(widget->selectedID){
        switch(models[widget->selectedID-1].type){
        case NOT:
            attr->viewable(0, 1, 0, 0, 0, 0, 0, 0, 0);
            break;
        case CUBE:
            attr->viewable(0, 0, 1, 0, 0, 0, 0, 1, 0);
            attr->RLabel->setText("边长");
            attr->R->setValue(models[widget->selectedID-1].entityAttr[0]);
            break;
        case PRISMOID:
            attr->viewable(0, 0, 1, 1, 1, 1, 1, 0, 0);
            attr->faceNum->setValue(models[widget->selectedID-1].entityAttr[0]);
            attr->faceNumLabel->setText("侧面数");
            attr->faceNum->setMinimum(0);
            attr->height->setValue(models[widget->selectedID-1].entityAttr[1]);
            attr->botR->setValue(models[widget->selectedID-1].entityAttr[2]);
            attr->bottomRLabel->setText("底面外接圆半径");
            attr->topR->setValue(models[widget->selectedID-1].entityAttr[3]);
            attr->topRLabel->setText("顶面外接圆半径");
            break;
        case SPHERE:
            attr->viewable(0, 0, 1, 0, 0, 0, 0, 1, 1);
            attr->RLabel->setText("半径");
            attr->R->setValue(models[widget->selectedID-1].entityAttr[0]);
            attr->density->setValue(models[widget->selectedID-1].entityAttr[1]);
            break;
        case CYLINDER:
            attr->viewable(0, 0, 1, 0, 0, 1, 1, 1, 0);
            attr->RLabel->setText("半径");
            attr->R->setValue(models[widget->selectedID-1].entityAttr[2]);
            attr->faceNumLabel->setText("线密度");
            attr->faceNum->setMinimum(30);
            attr->faceNum->setValue(models[widget->selectedID-1].entityAttr[0]);
            attr->height->setValue(models[widget->selectedID-1].entityAttr[1]);
        case CONE:
            attr->viewable(0, 0, 1, 0, 0, 1, 1, 1, 0);
            attr->RLabel->setText("半径");
            attr->R->setValue(models[widget->selectedID-1].entityAttr[2]);
            attr->faceNumLabel->setText("线密度");
            attr->faceNum->setMinimum(30);
            attr->faceNum->setValue(models[widget->selectedID-1].entityAttr[0]);
            attr->height->setValue(models[widget->selectedID-1].entityAttr[1]);
        default: break;
        }
    }
    else
        attr->viewable(1, 0, 0, 0, 0, 0, 0, 0, 0);
}

void MainWindow::reshapeEntity()
{
    if(models[widget->selectedID-1].type == CUBE){
        model Cube = createCube(attr->R->value());
        Cube.type = CUBE;
        models[widget->selectedID-1] = Cube;
    }
    else if(models[widget->selectedID-1].type == PRISMOID){
        Prismoid prismoid(attr->faceNum->value(), attr->height->value(), attr->botR->value(), attr->topR->value());
        model P = prismoid.createPrismoid();
        P.type = PRISMOID;
        models[widget->selectedID-1] = P;
    }
    else if(models[widget->selectedID-1].type == SPHERE){
        Sphere sphere(attr->R->value(), attr->density->value());
        model s = sphere.createSphere();
        s.type = SPHERE;
        models[widget->selectedID-1] = s;
    }
    else if(models[widget->selectedID-1].type == CYLINDER){
        Prismoid prismoid(attr->faceNum->value(), attr->height->value(), attr->R->value(), attr->R->value());
        model P = prismoid.createPrismoid();
        P.type = CYLINDER;
        models[widget->selectedID-1] = P;
    }
    else if(models[widget->selectedID-1].type == CONE){
        Cone cone(attr->R->value(), attr->height->value(), attr->faceNum->value());
        model C = cone.createCone();
        C.type = CONE;
        models[widget->selectedID-1] = C;
    }
}


void MainWindow::on_actionScreen_Capture_triggered()
{
  QImage image = widget->grabFrameBuffer(false);

  QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("save file"),
                                                    "E:/Study/ComputerGraphics/qtopengl/OBJ/export",
                                                    tr("PNG file(*.png)"));

  image.save(fileName, "png");
}

void MainWindow::on_actionExport_OBJ_File_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("save file"),
                                                    "E:/Study/ComputerGraphics/qtopengl/OBJ/export",
                                                    tr("OBJ file(*.obj)"));
    exportOBJ(fileName);
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    curItem = ui->treeWidget->itemAt(pos);
    if(curItem==NULL || curItem->parent()==NULL)
        return;
    QMenu *popMenu = new QMenu(this);
    popMenu->addAction(ui->actionSelect);
    popMenu->addAction(ui->actionDelete);
    popMenu->exec(QCursor::pos());
}

void MainWindow::on_actionSelect_triggered()
{
    QTreeWidgetItem *parent = curItem->parent();
    if(parent == NULL)
        return;
    int index = parent->indexOfChild(curItem);
    int currentModelID;
    if(parent->text(0) == "Objects")
       currentModelID = objAttr[index].modelIndex;
    else
       currentModelID = entityAttr[index].modelIndex;

    emit sendSelectOBJ(currentModelID + 1);
}

void MainWindow::on_actionDelete_triggered()
{
    QTreeWidgetItem *parent = curItem->parent();
    if(parent == NULL)
        return;
    int index = parent->indexOfChild(curItem);
    int currentModelID;
    if(parent->text(0) == "Objects")
       currentModelID = objAttr[index].modelIndex;
    else
       currentModelID = entityAttr[index].modelIndex;

    vector<model>::iterator it = models.begin() + currentModelID;
    models.erase(it);

    delete curItem;
}