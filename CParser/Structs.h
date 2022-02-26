#pragma once
//The syntax parser project for simple c++ runtime reflection

#include <string>
#include <vector>

namespace Parser {
	struct datatypes {
		virtual std::string toString();
	};

	//The type of each primitive variable
	//int, char, float, double, bool, etc
	struct primitive : public datatypes {
		std::vector<std::string> modifiers;
		std::string type, name;

		primitive(std::string t_type, std::string t_name, std::vector<std::string> t_modifiers = {}) : type(t_type), name(t_name), modifiers(t_modifiers) {};

		std::string toString() {
			std::string output = "";
			for (std::string x : modifiers) output += x + " ";
			return output + type + " " + name;
		}
	};

	//derived types of data
	//arrays, pointers, etc
	struct complex : public datatypes {
		primitive prim;
		std::string modifier;

		complex(primitive t_prim, std::string t_modifier) : prim(t_prim), modifier(t_modifier) {};

		std::string toString() { return prim.toString() + " " + modifier; }
	};

	//TODO
	//enum datatypes
	struct enums : public datatypes {
		std::string toString() { return ""; }
	};

	struct functions : public datatypes {
		//complex/primitive/class return, string name, list of complex/primitive/class parameters

		datatypes returnType;
		std::string name;
		std::vector<datatypes> params;

		functions(datatypes t_returnType, std::string t_name, std::vector<datatypes> t_params = {}) : returnType(t_returnType), name(t_name), params(t_params) {};

		std::string toString() {
			std::string output = returnType.toString() + " " + name + "(";
			for (datatypes x : params) output += x.toString() + ", ";
			return output + ");";
		}
	};

	struct classes : public datatypes {
		//may contain all of the above
		std::vector<datatypes> m_public, m_private, m_protected;
		std::string name, modifier;
		std::vector<std::string> bases, friends;

		classes(
			std::string t_name, std::string t_modifier = "",
			std::vector<std::string> t_bases = {}, std::vector<std::string> t_friends = {},
			std::vector<datatypes> t_public = {}, std::vector<datatypes> t_private = {}, std::vector<datatypes> t_protected = {})

			: name(t_name), modifier(t_modifier),
			bases(t_bases), friends(t_friends),
			m_public(t_public), m_private(t_private), m_protected(t_protected) {};

		std::string toString() {
			std::string output = modifier + " " + name + " {\n";
			output += "public:\n";
			for (datatypes x : m_public) output += "\t" + x.toString() + ";\n";
			output += "private:\n";
			for (datatypes x : m_private) output += "\t" + x.toString() + ";\n";
			output += "protected:\n";
			for (datatypes x : m_protected) output += "\t" + x.toString() + ";\n";
			for (std::string x : friends) output += "\tfriend class " + x + "\n";
			return output + "};";
		}
	};

	struct namespaces : public datatypes {
		//may contain all of the above
		std::string name;
		std::vector<datatypes> members;

		namespaces(std::string t_name, std::vector<datatypes> t_members = {}) : name(t_name), members(t_members) {};

		std::string toString() {
			std::string output = "namespace " + name + " {\n";
			for (datatypes x : members) output += "\t" + x.toString() + "\n";
			return output + "}";
		}
	};

	//TODO: Add union support
}