#include "XMLWriter.h"
#include <sstream>

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

CXMLWriter::~CXMLWriter() = default;

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
        // Self-close the tag if it is a self-closing element
        if (entity.DType == SXMLEntity::EType::CompleteElement) {
            output << "/>";
        } else {
            output << ">";
        }
    }
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        output << "</" << entity.DNameData << ">";
    }
    else if (entity.DType == SXMLEntity::EType::CompleteElement) {
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
        output << "/>";
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());

    return Sink->Write(outputVec) > 0;
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}