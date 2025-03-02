#include "XMLWriter.h"
#include <sstream>

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;
    std::ostringstream output;

    if (entity.DType == SXMLEntity::EType::CompleteElement) {
        // Build both the start tag and inner text.
        output << "<" << entity.DNameData;
        std::string innerText;
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"";
            std::ostringstream escaped;
            for (char c : attr.second) {
                if (c == '&') {
                    escaped << "&amp;";
                } else if (c == '<') {
                    escaped << "&lt;";
                } else if (c == '>') {
                    escaped << "&gt;";
                } else if (c == '\"') {
                    escaped << "&quot;";
                } else if (c == '\'') {
                    escaped << "&apos;";
                } else {
                    escaped << c;
                }
            }
            std::string ev = escaped.str();
            output << ev << "\"";
            innerText += ev;  // Always accumulate the escaped value.
        }
        output << ">" << innerText << "</" << entity.DNameData << ">";
    } 
    else if (entity.DType == SXMLEntity::EType::StartElement) {
        // For elements that will have children, output only the start tag.
        output << "<" << entity.DNameData;
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"";
            for (char c : attr.second) {
                if (c == '&')
                    output << "&amp;";
                else if (c == '<')
                    output << "&lt;";
                else if (c == '>')
                    output << "&gt;";
                else if (c == '\"')
                    output << "&quot;";
                else if (c == '\'')
                    output << "&apos;";
                else
                    output << c;
            }
            output << "\"";
        }
        output << ">";
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