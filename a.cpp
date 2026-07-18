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
// __attribute_nonnull__((1))
// __attribute_deprecated__
// __attribute_warn_unused_result__
int	func(char *str);

int	func(char *str)
{
	if (!str)
		return (0);
	return (1);
}

int	main(void)
{
	char	*s = nullptr;

	func(nullptr);
	return (0);
}
