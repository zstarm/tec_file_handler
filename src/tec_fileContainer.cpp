#include "tec_fileContainer.h"

namespace tec {
	//--------------------------------------------------------------------------------------------
	// TECPLOT FILE CONTENT
	//--------------------------------------------------------------------------------------------
	fileContainer::fileContainer() : fType(fileTypeFlag::full), title("N/A") {}

	fileContainer::~fileContainer() {}

	void fileContainer::print_headerDetails() {
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		std::cout << "TECPLOT FILE HEADER DETAILS" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		//title
		std::cout << "TITLE: " << title << std::endl;

		//file type
		std::cout << "FILE TYPE: "; 
		switch(fType) {
			case fileTypeFlag::full:
				std::cout << "FULL" << std::endl;
				break;
			case fileTypeFlag::grid:
				std::cout << "GRID" << std::endl;
				break;
			case fileTypeFlag::solution:
				std::cout << "SOLUTION" << std::endl;
				break;
		}

		//variable list
		std::cout << "VARIABLES (#SZ): ";
		for(int v = 0; v < vars.size(); v++) {
			std::cout << vars[v].name;
			std::cout << "(" << vars[v].subzoneData.size() << ")";
			//ternary operator to determine if at the end of the variable list or not
			v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
		}
	}

	void fileContainer::print_zoneDetails(int zidx) {
		std::cout << "TECPLOT ZONE #" << zidx+1 << " DETAILS" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		
		//zone title
		std::cout << "ZONE TITLE: " << zoneDetails[zidx].zoneTitle << std::endl;

		//zone type
		std::cout << "ZONE TYPE: ";
		zoneDetails[zidx].zoneType == zoneTypeFlag::ordered ? std::cout << "ORDERED" : std::cout << "FINITE ELEMENT";
		std::cout << std::endl;

		//zone formatting/datapacking 
		std::cout << "ZONE FORMATTING: ";
		zoneDetails[zidx].dataPacking == formatFlag::point ? std::cout << "POINT" : std::cout << "BLOCK";
		std::cout << std::endl;

		std::cout << "ZONE DIMENSIONS: I = " << zoneDetails[zidx].I << ", J = " << zoneDetails[zidx].J << ", K = " << zoneDetails[zidx].K << std::endl;

		//transient datasets only
		if(zoneDetails[zidx].strandID) {
			//strand ID
			std::cout << "ZONE STRAND ID: " << zoneDetails[zidx].strandID << std::endl;
			std::cout << "ZONE SOLUTION TIME: " << zoneDetails[zidx].solutionTime << std::endl;
		}

	}

	void fileContainer::print_zoneData(int zidx) {
		std::cout << "TECPLOT ZONE #" << zidx+1 << " DATA" << std::endl;
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		for(int v = 0; v < vars.size(); v++) {
			std::cout << vars[v].name;
			std::cout << "(";
			if(zoneDetails[zidx].zone_sharedVars[v]) {
				std::cout << "SHARED)";
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
				continue;
			}
			else if(zoneDetails[zidx].zone_passiveVars[v]) {
				std::cout << "PASSIVE)";
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
				continue;
			}
			switch(vars[v][zidx].T) {
				case dataTypeFlag::singlePrecision:
					std::cout << "SINGLE, ";
					break;
				case dataTypeFlag::doublePrecision:
					std::cout << "DOUBLE, ";
					break;
				case dataTypeFlag::int32:
					std::cout << "LONGINT, ";
					break;
				case dataTypeFlag::int16:
					std::cout << "SHORTINT, ";
					break;
				case dataTypeFlag::byte:
					std::cout << "BYTE, ";
					break;
				default:
					throw containerError("issue getting data type for variable: " + vars[v].name + " at zone" + std::to_string(zidx) + " to display");
			}
			if(zoneDetails[zidx].zone_varLoc[v]) {
				std::cout << "NODE";
			}
			else {
				std::cout << "CELLCENTER";
			}
			std::cout << ")";
			//ternary operator to determine if at the end of the variable list or not
			v != vars.size()-1 ? std::cout << ", " : std::cout << std::endl;
		}
		std::cout << "---------------------------------------------------------------------------------" << std::endl;
		for(int i = 0; i < zoneDetails[zidx].get_size(); i++) {
			for(int v = 0; v < vars.size(); v++) {
				if(zoneDetails[zidx].zone_sharedVars[v]) {
					if(!v) {
						continue;
					}
					v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
					continue;
				}
				else if(zoneDetails[zidx].zone_passiveVars[v]) {
					if(!v) {
						continue;
					}
					v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
					continue;
				}
				
				if(!zoneDetails[zidx].zone_varLoc[v]) {
					//if cell centered make sure index isn't above number of cell values
					int nCells = zoneDetails[zidx].get_size(false);
					if(i >= nCells) {
						if(!v) {
							continue;
						}
						v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
						continue;
					}
				}
				
				switch(vars[v][zidx].T) {
					case dataTypeFlag::singlePrecision:
						std::cout << vars[v][zidx].get_float(i);
						break;
					case dataTypeFlag::doublePrecision:
						std::cout << vars[v][zidx].get_double(i);
						break;
					case dataTypeFlag::int32:
						std::cout << vars[v][zidx].get_int32(i);
						break;
					case dataTypeFlag::int16:
						std::cout << vars[v][zidx].get_int16(i);
						break;
					case dataTypeFlag::byte:
						std::cout << vars[v][zidx].get_byte(i);
						break;
					default:
						throw containerError("issue getting data type for variable: " + vars[v].name + " at zone" + std::to_string(zidx) + " to display");
				}
				//ternary operator to determine if at the end of the variable list or not
				v != vars.size()-1 ? std::cout << "\t" : std::cout << std::endl;
			}
		}
	}

	void fileContainer::print_fileDetails(bool include_data) {
		try {
			print_headerDetails();
			std::cout << "\n" << std::endl;	
			for(int z = 0; z < zoneDetails.size(); z++) {
				print_zoneDetails(z);
				std::cout << "\n" << std::endl;	
				if(include_data) {
					print_zoneData(z);
					std::cout << "\n" << std::endl;	
				}
			}
		}
		catch(containerError const &e) {
			std::cout << "\nCONTAINER ERROR: " << e.what() << " (CODE: " << e.code + e.secondary_code << ")" << std::endl;
		}
	}

	void fileContainer::operator[](int vidx) {
		std::cout << "variable name at " << vidx << " is " << vars[vidx].name << std::endl;
	}

	void fileContainer::operator[](std::string vname) {
		try {
			int vidx = var_map.at(vname);
			std::cout << "variable name given key " << vname << " is " << vars[vidx].name << std::endl;
		}

		catch(std::out_of_range const& e) {
			std::cout << "The variable key \"" << vname << "\" does not correspond to the name any variable" << std::endl;
			std::cout << "Please using one of the following names as a key or the corrsponding index number:" << std::endl;
			for(int v = 0; v < vars.size(); v++) {
				std::cout << v << ": ";
				std::cout << vars[v].name << std::endl;
			}
		}
		
	}
}
