#include <iostream>

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
// __attribute_nonnull__((1))
void	func(char *str)
{

}

int	main(void)
{
	char	*s = nullptr;

	func(nullptr);
	return (0);
}
