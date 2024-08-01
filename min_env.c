#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	char *result = malloc((len1 + len2 + 1) * sizeof(char));
	if (!result)
	{
		return (NULL);
	}
	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}


char	**create_minimal_env(void)
{
	char	**env_min;
	char	cwd[1024];
	char	*temp;

	env_min = malloc(4 * sizeof(char *));
	if (!env_min)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	temp = getcwd(cwd, sizeof(cwd));
	if (temp == NULL)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	env_min[0] = ft_strjoin("PWD=", temp);
	if (!env_min[0])
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	env_min[1] = ft_strjoin("SHLVL=", "1");
	if (!env_min[1])
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	env_min[2] = ft_strjoin("_=", "/usr/bin/env");
	if (!env_min[2])
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	env_min[3] = NULL;
	return (env_min);
}

void	free_environment(char **env_min)
{
	int i;

	i = 0;
	while (env_min[i] != NULL)
	{
		free(env_min[i]);
		i++;
	}
	free(env_min);
}

int	main(void)
{
	char	**envmin;
	int		i;

	envmin = create_minimal_env();
	i = 0;
	while (envmin && envmin[i])
	{
		printf("%s\n", envmin[i]);
		i++;
	}
	free_environment(envmin);
	return (0);
}
