#include "dxf.hpp"

void
  core::utils::dxf::write(
    _p(blocks, utils::polygon::polygon_list const&),
    _p(path  , std::string const&),
    _p(rx, double),
    _p(ry, double)
  ) {
    DL_Dxf dxf;

    auto writer = dxf.out(path.c_str(), DL_Codes::AC1015);
    if (writer == nullptr)
      return;

    dxf.writeHeader(*writer);
    writer->dxfString(9, "$INSUNITS");
    writer->dxfInt(70, 5);
    writer->dxfString(9, "$DIMEXE");
    writer->dxfReal(40, 1.25);
    writer->dxfString(9, "$TEXTSTYLE");
    writer->dxfString(7, "Standard");
    // vector variable:
    writer->dxfString(9, "$LIMMIN");
    writer->dxfReal(10, 0.0);
    writer->dxfReal(20, 0.0);
    writer->sectionEnd();

    writer->sectionTables();
    dxf.writeVPort(*writer);
    writer->tableEnd();

    writer->tableLayers(0);
    writer->tableEnd();

    dxf.writeStyle(*writer);
    dxf.writeView(*writer);
    dxf.writeUcs(*writer);
    writer->tableAppid(1);
    writer->tableAppidEntry(0x12);
    writer->dxfString(2, "ACAD");
    writer->dxfInt(70, 0);
    writer->tableEnd();

    dxf.writeBlockRecord(*writer);
    writer->tableEnd();
    writer->sectionEnd();

    writer->sectionBlocks();
    writer->sectionEnd();

    writer->sectionEntities();
    for (int block_index = 0; block_index < blocks.size(); ++block_index) {
      auto block = blocks[block_index];
      auto block_name = "block-" + std::to_string(block_index);

      for (int i = 0, j = 1; i < block.size(); ++i, j++) {
        if (i == block.size() - 1)
          j = 0;

        auto const &point = block[i], next = block[j];

        dxf.writeLine(
          *writer,
          DL_LineData(
            point.x * rx, point.y * ry, 0,
            next.x * rx, next.y * ry, 0
          ),
          DL_Attributes("", 256, -1, "BYLAYER")
        );
      }
    }
    writer->sectionEnd();

    dxf.writeObjects(*writer);
    dxf.writeObjectsEnd(*writer);

    writer->dxfEOF();
    writer->close();

    delete writer;
  }
