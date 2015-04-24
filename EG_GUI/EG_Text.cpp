
#include "EG_Text.h"


const int fontSize = 20;
const int fontSpace = -9;

EG_Text::EG_Text()
{
}



void EG_Text::initialize()
{
//    fontBase = new Texture("data/font.tga");
 //   buildTextureFont();


  //  m_fontQuads.resize(256);
    buildCharToIndexMapping();
    buildTextureFont();
}

void EG_Text::buildCharToIndexMapping()
{
    char c;

    m_charToIndexMapping[' '] = 0;
    m_charToIndexMapping['!'] = 1;
    m_charToIndexMapping['#'] = 2;
    m_charToIndexMapping['$'] = 3;
    m_charToIndexMapping['%'] = 4;
    m_charToIndexMapping['&'] = 5;

    m_charToIndexMapping[','] = 12;
    m_charToIndexMapping['-'] = 13;
    m_charToIndexMapping['.'] = 14;
    m_charToIndexMapping['/'] = 15;


    int start = 16;
    int index_start = 16;
    for(int i=0; i<10; i++)
    {
        c = '0' + i;
    //    cout << " char is " << c << endl;
        m_charToIndexMapping[c] = index_start+i;
    }

    /// Uppercase
    start = 'A'+1-1;
    index_start = 33;
    for(int i=start; i<start+26; i++)
    {
        c = char(i);
        m_charToIndexMapping[c] = index_start+i-start;
    }

    /// Lowercase
    start = 'a'+1-1;
    index_start = 65;
    for(int i=start; i<start+26; i++)
    {
        c = char(i);
    //    cout << index_start + i-start << endl;
        m_charToIndexMapping[c] = index_start + i-start;

    }

  //  cout << "mymap contains:";
  //  for ( auto it = m_charToIndexMapping.begin(); it != m_charToIndexMapping.end(); ++it )
 //       cout << " " << it->first << ":" << it->second << endl;
 //   cout << endl;

}



void EG_Text::buildTextureFont()
{
    fontTexture = EG_Utility::loadTexture("assets/font.jpg");
//    glBindTexture(GL_TEXTURE_2D, fontTexture->texID);

    float gap = 1/16;

    for(int i=0; i<256; i++)
    {
        float cx = (float)(i % 16);
        float cy = (float)(i / 16);

//        cout << i << " cx is " << cx << "cy is " << cy << endl;

        EG_Quad tempQuad;
        tempQuad.init(fontSize, fontSize, cx, cy, 0.0625);
 //       tempQuad.init(fontSize, cx, cy);
        m_fontQuads.push_back(tempQuad);
    }
}


void EG_Text::render(pipeline& m_pipeline, int x, int y, const char *in_text, ...)
{

    char text[256];

    va_list ap;

    va_start(ap, in_text);
        vsprintf(text, in_text, ap);
    va_end(ap);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);



    glBindTexture(GL_TEXTURE_2D, fontTexture);
    m_pipeline.pushMatrix();
//        m_pipeline.matrixMode(MODEL_MATRIX);
  //      m_pipeline.loadIdentity();

        m_pipeline.translate(x,y,0);

        int length = strlen(in_text);
        cout << "length " << length << endl;
        // for(int i=0; i<length; i++)
        for(int i=0; i<length; i++)
        {
            m_pipeline.pushMatrix();
                float x_offset = getTextWidthOffset(i);
        //        m_pipeline.translate(x_offset,y,0);
                m_pipeline.translate(x_offset,0,0);
           //     m_pipeline.rotate(180,0,0,1);

                char c = in_text[i];
      //          cout << "c is " << c << endl;
      //          cout << "mapping is " << m_charToIndexMapping[c] << endl << endl;
                int index = m_charToIndexMapping[c];

                m_fontQuads[index].quad->draw();
            m_pipeline.popMatrix();
        }

    m_pipeline.popMatrix();

}




void EG_Text::render(   pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID,
                        int x, int y, const char *in_text, ...)
{

    char text[256];

    va_list ap;

    va_start(ap, in_text);
        vsprintf(text, in_text, ap);
    va_end(ap);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBindTexture(GL_TEXTURE_2D, fontTexture);
    m_pipeline.pushMatrix();
//        m_pipeline.matrixMode(MODEL_MATRIX);
  //      m_pipeline.loadIdentity();

        m_pipeline.translate(x,y,0);

        int length = strlen(in_text);
        cout << "length " << length << endl;
        // for(int i=0; i<length; i++)
        for(int i=0; i<length; i++)
        {
            m_pipeline.pushMatrix();
                float x_offset = getTextWidthOffset(i);

                m_pipeline.translate(x_offset,0,0);

                char c = in_text[i];
                int index = m_charToIndexMapping[c];

                m_fontQuads[index].quad->draw();
            m_pipeline.popMatrix();
        }

    m_pipeline.popMatrix();

}





GLuint EG_Text::getFontTexture()
{
    return fontTexture;
}


int EG_Text::getTextWidthOffset(int i)
{
    return(fontSize+fontSpace) * i;
}


GLuint EG_Text::getTextWidth(const char* text)
{
    return(strlen(text) * fontSpace);
}


GLuint EG_Text::getTextHeight(const char* text)
{
    return fontSize;
}
