//#include <iostream>

// __attribute__((__nonnull__(1)))
// __attribute_nonnull__(1)
// __attribute_nonnull__((1))
// __attribute__((__nonnull__(1)))
// __attribute__((__hot__))
// __attribute__((__always_inline__))
// __always_inline
// __attribute__((__hot__))
// __attribute__((warn_unused_result))
// __attribute_warn_unused_result__
//void	func(char *str)
//{

//}

//int	main(void)
//{
//	char	*s = nullptr;

//	func(nullptr);
//	return (0);
//}

#include "parser/jsonconfig.hpp"
#include <iostream>

int main()
{
    try
    {
        JsonConfig config;

        nlohmann::json data = config.load_json("default.json");

        std::cout << "JSON cargado correctamente.\n";
        std::cout << data.dump(2) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}