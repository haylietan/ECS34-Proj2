#include "XMLWriter.h"
#include <sstream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Writes an XML entity to the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    std::ostringstream output;

    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;

        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"";
            for (char c : attr.second) {
                if (c == '&') {
                    output << "&amp;";
                }
                else if (c == '<') {
                    output << "&lt;";
                }
                else if (c == '>') {
                    output << "&gt;";
                }
                else if (c == '\"') {
                    output << "&quot;";
                }
                else if (c == '\'') {
                    output << "&apos;";
                }
                else {
                    output << c;
                }
            }
            output << "\"";
        }

        if (entity.DAttributes.empty()) {
            output << "/>";
        } else {
            output << ">";
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
    if(!Sink) {
        return false;
    }

    return true;
}