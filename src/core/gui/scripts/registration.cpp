#include "registration.hpp"

QString
  core::gui::scripts::registration::get_id() {
    const char* cmd =
      "cmd /c "
      "\""
      "wmic csproduct get UUID,IdentifyingNumber | more +1 & "
      "wmic idecontroller get Name,PNPDeviceID | more +1 & "
      "wmic memorychip get Caption,PartNumber,SerialNumber | more +1 & "
      "wmic baseboard get Product,Manufacturer,SerialNumber | more +1 & "
      "wmic cpu get Manufacturer,Caption,ProcessorId,PartNumber | more +1"
      "\"";
    auto pipe = popen(cmd, "r");
    //
    QString output;
    for (char buffer[1024]; fgets(buffer, sizeof buffer, pipe);)
      output += buffer;
    //
    pclose(pipe);

    return core::utils::hash::hash(output.toUtf8(), QCryptographicHash::Algorithm::Sha1).toUpper();
  }

void
  core::gui::scripts::registration::write_id() {
    auto path = QFileDialog
      ::getSaveFileName(
        nullptr,
        "REQUEST.txt",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/REQUEST.txt",
        "Text format (*.txt)"
      );

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;

    QTextStream output(&file);
    output << get_id();

    file.close();
  }

bool
  core::gui::scripts::registration::check_key(
    _p(key , QString const&),
    _p(seed, QString const&)
  ) {
    if (key.length() != 16)
      return false;

    std::mt19937 generator(seed.toUInt(nullptr, 16));

    QString randomized;
    for (auto const& chr : get_id()) {
      QChar random;
      do {
        random = char(float(generator()) / float(std::mt19937::max()) * float('Z' - chr.toLatin1()) + '0' - 1);
      } while (not (random.isDigit() || random.isLetter()));

      randomized += random;
    }

    return randomized.toUpper().startsWith(key.toUpper());
  }
