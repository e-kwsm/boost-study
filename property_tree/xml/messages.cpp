#include <locale>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

int main() {
  // create an empty property tree
  boost::property_tree::ptree pt;

  // create the root element
  boost::property_tree::ptree& root = pt.put("messages", "");

  // add child elements
  // absolute path
  pt.put("messages.note.<xmlattr>.id", "501");
  pt.put("messages.note.to", "Tove");
  pt.put("messages.note.from", "Jani");
  pt.put("messages.note.heading", "Reminder");
  pt.put("messages.note.body", "Don't forget me this weekend!");
  // relative path
  boost::property_tree::ptree& child = root.add("note", "");
  child.put("<xmlattr>.id", "502");
  child.put("to", "Jani");
  child.put("from", "Tove");
  child.put("heading", "Re: Reminder");
  child.put("body", "I will not");

  // output
  boost::property_tree::write_xml(
      "messages.xml", pt, std::locale(),
      boost::property_tree::xml_writer_make_settings<std::string>(' ', 2));

  return 0;
}
