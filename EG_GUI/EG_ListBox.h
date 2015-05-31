#ifndef EG_LIST_BOX_H
#define EG_LIST_BOX_H

#include <vector>

#include "EG_Rect.h"
#include "EG_Control.h"

using namespace std;

class EG_ListBox : public EG_Control
{
    public:
        EG_ListBox();
        EG_ListBox(int x, int y, int width, int height);

        virtual void initColoredQuad();

        void addItem(string item);
        void removeItem(int index);
        void setCurrent(int index);

        int getIndex();
        int getCount();



//        void update(int posX, int posY, int width, int height);
        using EG_Control::update;
        virtual bool update(MouseState & state);


        virtual void render (pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID);
        virtual int getType();

    protected:
        int m_curIndex;

        EG_Rect         m_curQuadData;
        EG_QuadModelABS m_curQuadModel;



        vector<string> m_items;
};




#endif // EG_BUTTON_H
