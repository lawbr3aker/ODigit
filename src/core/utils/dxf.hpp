#pragma once

#include <dxflib/dl_dxf.h>

#include "./polygon.hpp"

#include "../macros.h"

namespace core::utils::dxf {
  void
    write(
      _p(blocks, utils::polygon::polygon_list const&),
      _p(path  , std::string const&),
      _p(rx, double) = 1,
      _p(ry, double) = 1
    );

//  class block: public DRW_Block {
//   public:
//    _p(entities, std::list<DRW_Entity>);
//  };
//
//  class writer: public DRW_Interface {
//   public:
//    writer(
//      _p(polygons, utils::polygon::polygon_list)
//    );
//
//    virtual
//    void
//      addHeader(
//        _p(data, DRW_Header const*)
//      ) override {}
//    virtual
//    void
//      addLType(
//        _p(data, DRW_LType const&)
//      ) override {}
//    virtual
//    void
//      addLayer(
//        _p(data, DRW_Layer const&)
//      ) override {}
//    virtual
//    void
//      addDimStyle(
//        _p(data, DRW_Dimstyle const&)
//      ) override {}
//    virtual
//    void
//      addVport(
//        _p(data, DRW_Vport const&)
//      ) override {}
//    virtual
//    void
//      addTextStyle(
//        _p(data, DRW_Textstyle const&)
//      ) override {}
//    virtual
//    void
//      addAppId(
//        _p(data, DRW_AppId const&)
//      ) override {}
//    virtual
//    void
//      addBlock(
//        _p(data, DRW_Block const&)
//      ) override;
//    virtual
//    void
//      setBlock(
//        _p(data, int const)
//      ) override {}
//    virtual
//    void
//      endBlock(
//      ) override {}
//
//    virtual
//    void
//      addPoint(
//        _p(data, DRW_Point const&)
//      ) override {
//      _block.push_back(new DRW_Point(data));
//    }
//    virtual
//    void
//      addRay(
//        _p(data, DRW_Ray const&)
//      ) override {
//      _block.push_back(new DRW_Ray(data));
//    }
//    virtual
//    void
//      addXline(
//        _p(data, DRW_Xline const&)
//      ) override {}
//    virtual
//    void
//      addArc(
//        _p(data, DRW_Arc const&)
//      ) override {
//      _block.push_back(new DRW_Arc(data));
//    }
//    virtual
//    void
//      addCircle(
//        _p(data, DRW_Circle const&)
//      ) override {
//      _block.push_back(new DRW_Circle(data));
//    }
//
//    virtual void addEllipse(const DRW_Ellipse &data) override {
//
//    }
//
//    virtual void addLWPolyline(const DRW_LWPolyline &data) override {
//
//    }
//
//    virtual void addPolyline(const DRW_Polyline &data) override {
//
//    }
//
//    virtual void addSpline(const DRW_Spline *data) override {
//
//    }
//
//    virtual void addKnot(const DRW_Entity &data) override {
//
//    }
//
//    virtual void addInsert(const DRW_Insert &data) override {
//
//    }
//
//    virtual void addTrace(const DRW_Trace &data) override {
//
//    }
//
//    virtual void add3dFace(const DRW_3Dface &data) override {
//
//    }
//
//    virtual void addSolid(const DRW_Solid &data) override {
//
//    }
//
//    virtual void addMText(const DRW_MText &data) override {
//
//    }
//
//    virtual void addText(const DRW_Text &data) override {
//
//    }
//
//    virtual void addDimAlign(const DRW_DimAligned *data) override {
//
//    }
//
//    virtual void addDimLinear(const DRW_DimLinear *data) override {
//
//    }
//
//    virtual void addDimRadial(const DRW_DimRadial *data) override {
//
//    }
//
//    virtual void addDimDiametric(const DRW_DimDiametric *data) override {
//
//    }
//
//    virtual void addDimAngular(const DRW_DimAngular *data) override {
//
//    }
//
//    virtual void addDimAngular3P(const DRW_DimAngular3p *data) override {
//
//    }
//
//    virtual void addDimOrdinate(const DRW_DimOrdinate *data) override {
//
//    }
//
//    virtual void addLeader(const DRW_Leader *data) override {
//
//    }
//
//    virtual void addHatch(const DRW_Hatch *data) override {
//
//    }
//
//    virtual void addViewport(const DRW_Viewport &data) override {
//
//    }
//
//    virtual void addImage(const DRW_Image *data) override {
//
//    }
//
//    virtual void linkImage(const DRW_ImageDef *data) override {
//
//    }
//
//    virtual void addComment(const char *comment) override {
//
//    }
//
//    virtual void writeHeader(DRW_Header &data) override {
//
//    }
//
//    virtual
//    void
//      writeBlocks(
//      ) override;
//
//    virtual void writeBlockRecords() override {
//
//    }
//
//    virtual void writeLTypes() override {
//
//    }
//
//    virtual void writeLayers() override {
//
//    }
//
//    virtual void writeTextstyles() override {
//
//    }
//
//    virtual void writeVports() override {
//
//    }
//
//    virtual void writeDimstyles() override {
//
//    }
//
//    virtual void writeAppId() override {
//
//    }
//
//    virtual void writeEntities() override;
//
//   private:
//    _p(_block, std::list<DRW_Entity *>);
//  };
}