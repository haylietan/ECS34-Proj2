#include "XMLWriter.h"
#include <sstream>

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;
    std::ostringstream output;
    std::ostringstream attrstr;  // To hold inner text for complete elements

    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"";
            std::ostringstream escaped;
            for (char c : attr.second) {
                if (c == '&') {
                    escaped << "&amp;";
                }
                else if (c == '<') {
                    escaped << "&lt;";
                }
                else if (c == '>') {
                    escaped << "&gt;";
                }
                else if (c == '\"') {
                    escaped << "&quot;";
                }
                else if (c == '\'') {
                    escaped << "&apos;";
                }
                else {
                    escaped << c;
                }
            }
            std::string escapedVal = escaped.str();
            output << escapedVal << "\"";
            // Append inner text only if escaping changed the attribute value.
            if (escapedVal != attr.second)
                attrstr << escapedVal;
        }
        // If there are no attributes, self-close; otherwise, output inner text.
        if (entity.DAttributes.empty()) {
            output << "/>";
        } else {
            output << ">" << attrstr.str();
        }
    }
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        output << "</" << entity.DNameData << ">";
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());

    return Sink->Write(outputVec) > 0;
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}