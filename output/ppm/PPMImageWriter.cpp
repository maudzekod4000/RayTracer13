#include "PPMImageWriter.h"

#include <fstream>
#include <string>
#include <format>
#include <assert.h>

#include "PPMImageMeta.h"
#include "PPMImage.h"

PPMImageWriter::PPMImageWriter(const PPMImage& image): image(image) {}

__declspec(safebuffers)
void PPMImageWriter::write(std::ostream& output) const
{
    assert(!image.buffer.empty());
    image.metadata.writeHeaders(output);

    std::string buffer;

    for (const auto& row : image.buffer) {
        for (const auto& color : row) {
            std::format_to(std::back_inserter(buffer), "{} {} {} ", color.r, color.g, color.b);
        }
        buffer += '\n';
    }

    output << buffer;
}
