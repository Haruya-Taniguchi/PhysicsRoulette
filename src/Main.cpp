# include <Siv3D.hpp>
# include <HamFramework.hpp>
# define CLASS 0
# define NUM 1
# define BREAK 2
int mode = CLASS;
int grade, number, max;
MultiPolygon MakePolygons(const Font& font, const String& text)
{
    Image image(1200, 400, Color(0, 0));
    
    font(text).overwrite(image, 0, 0, Palette::White);
    if(mode == NUM)
        return image.alphaToPolygons().simplified(1).movedBy(-370, -350).scaled(0.03);
    else
        return image.alphaToPolygons().simplified(1).movedBy(100, -350).scaled(0.03);
}

void Main()
{
    Window::Resize(1280, 720);
    Graphics::SetBackground(Color(0, 0, 0));
    
    P2World world;
    const P2Body line = world.createLine(Vec2(0, 0), Line(-24, 0, 24, 0), P2Material(1, 0.1, 1.0), none, P2BodyType::Static);
    Array<P2Body> bodies;
    
    Camera2D camera(Vec2(0, -5), 38);
    camera.enableKeyControl(false);
    
    const Font font(230, Typeface::Bold);
    const Font font2(128, Typeface::Bold);
    String text=U"test";
    int32 generation = 0;
    HashTable<P2BodyID, int32> table;
    
    while (System::Update())
    {
        //TextInput::UpdateText(text);
        world.update(System::DeltaTime(0.05), 30, 10);
        
        //camera.update();
        
        auto t = camera.createTransformer();
        
        for (const auto& body : bodies)//物体を描画
        {
            body.draw(Palette::White);
        }
        
        line.draw(Palette::Black);

        {
            Transformer2D trans(Mat3x2::Scale(0.025));
            font2(U"クラス").draw(-500, -500, Color(255));
            font2(U"出席番号").draw(60, -500, Color(255));
            if(mode==CLASS){
                font(Random(1,5),U"I").draw(-420, -350, Color(255));
                font(Random(1,44)).draw(150, -350, Color(255));
            }else if(mode==NUM){
                font(Random(1,44)).draw(150, -350, Color(255));
            }
        }
        
        if(MouseL.down()){
            if (mode == CLASS)
            {
                mode = NUM;
                grade = Random(1,5);//学年
                
                text = Format(grade,U"I");
            }else if(mode == NUM){
                mode = CLASS;
                switch(grade){//各学年の出席番号をmaxに代入
                    case 1:
                        max = 40;
                        break;
                    case 2:
                        max = 45;
                        break;
                    case 3:
                        max = 38;
                        break;
                    case 4:
                        max = 42;
                        break;
                    case 5:
                        max = 36;
                        break;
                }
                number = Random(1,max);
                text = Format(number);
                mode = BREAK;
            }else if(mode == BREAK){
                text.clear();
                bodies.clear();
                mode = CLASS;
            }
            
            for (const auto& poly : MakePolygons(font, text))
            {
                const Vec2 centroid = poly.centroid();
                
                bodies << world.createPolygon(centroid, poly.movedBy(-centroid), P2Material(1, 0.0, 0.8));//密度、反発力、摩擦
                
                table[bodies.back().id()] = generation;
            }
            
            ++generation;
            
        }
        if(MouseR.down()){bodies.clear();}
        camera.draw(Palette::Orange);
    }
}
