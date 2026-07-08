#include "parser/jsonconfig.hpp"

#include <fstream> // Libreria que permite trabajar con archivos, sin ella no podemos abrir un .json o .txt
#include <sstream> // Para pillar cadenas de texto de manera secuencial
#include <stdexcept> //para indiicar cuando a habido un error

std::string	JsonConfig::read_file(const std::string& path)
{
	std::ifstream	file(path); //La i viene de input

	if (!file.is_open())
		return ("");

	std::stringstream	buffer; // El buffer va a ir almacenando todos los datos que se leen en ek archivo
	buffer << file.rdbuf(); //Devuelve el contenido del archivo

	return (buffer.str());
}

std::string	JsonConfig::remove_comments(const std::string& input) //Recibira el contendio completo del archivo y eliminara comentarios
{
	return (input);
}

std::string	JsonConfig::remove_trailing_commas(const std::string& input) //Eliminara las comas finales
{
	return (input);
}

void	JsonConfig::validate(const std::string& input) //Comprobar que todo el archvio tiene sentido, ejem: {} y debera validar mas cosas como laa comillas o caracteres raros...
{
	(void)input;
}

std::string	JsonConfig::load_json(const std::string& path)
{
	std::string	content;

	content = read_file(path);
	content = remove_comments(content);
	content = remove_trailing_commas(content);

	validate(content);

	return (content);
}