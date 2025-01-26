#ifndef COLOUROBJ_H_
#define COLOUROBJ_H_

#include <ngl/Obj.h>
#include <ngl/Vec3.h>

// extend the Obj class to calculate barycentric coordinates
class BaryObj : public  ngl::Obj
{
  public :
    BaryObj() noexcept : Obj() {}
    explicit BaryObj( const std::string& _fname ,ngl::Obj::CalcBB _calcBB=ngl::Obj::CalcBB::True ) noexcept;

    void createBaryVAO() noexcept;
    //void draw();

  protected :
    bool parseVertex(std::vector<std::string> &_tokens) noexcept override ;

    std::vector <ngl::Vec3> m_baryCords;
};


#endif
