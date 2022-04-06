/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 13:49:34 by taejkim           #+#    #+#             */
/*   Updated: 2022/04/06 21:07:07 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx/mlx.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>

#define HEIGHT 960
#define WIDTH 640
#define TEX_SIZE 64

#define X_EVENT_KEY_PRESS 2
#define X_EVENT_KEY_RELEASE 3
#define X_EVENT_KEY_EXIT 17

#define KEY_W 13
#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_UP 126
#define KEY_DOWN 125
#define KEY_ESC 53

#define NO 0
#define SO 1
#define WE 2
#define EA 3
#define F 4
#define C 5



typedef struct s_file
{
	char	*str;
	char	**split;
	char	*info[6];
	char	**map;
	int		map_w;
	int		map_h;
	int		c_flag;
}	t_file;

typedef struct s_key
{
	int		w;
	int		a;
	int		s;
	int		d;
	int		left;
	int		right;
}	t_key;

typedef struct s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_key	key;
	t_img	img;
	int		**buf;
	int		*texture[4];
	int		ceiling;
	int		floor;
	char	**map;
	int		map_w;
	int		map_h;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	movespeed;
	double	rotspeed;
}	t_game;


// libft //////////////////////////////////////////////////////////
int	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	*ft_strjoin_free_s1(char *s1, char *s2)
{
	char	*ptr;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	j = -1;
	while (s1[++j])
		ptr[i++] = s1[j];
	j = -1;
	while (s2[++j])
		ptr[i++] = s2[j];
	ptr[i] = 0;
	free(s1);
	return (ptr);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char *s1_u;
	const unsigned char *s2_u;
	size_t				i;

	s1_u = (unsigned char *)s1;
	s2_u = (unsigned char *)s2;
	i = 0;
	while (s1_u[i] && s2_u[i] && (i < n))
	{
		if (s1_u[i] != s2_u[i])
			return (s1_u[i] - s2_u[i]);
		++i;
	}
	if (i == n)
		return (0);
	if (s1_u[i] != s2_u[i])
		return (s1_u[i] - s2_u[i]);
	else
		return (0);
}

static int	get_size(char const *s, char c)
{
	int res;
	int check;
	int i;

	res = 0;
	check = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			if (check == 0)
				++res;
			check = 1;
		}
		else
			check = 0;
		++i;
	}
	return (res);
}

static char	*get_ptr_child(char const *s, char c, int i)
{
	char	*res;
	int		start;
	int		end;
	int		j;

	start = i;
	while (s[i])
	{
		if (s[i] == c)
			break ;
		++i;
	}
	end = i;
	res = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!res)
		return (NULL);
	j = 0;
	while (start < end)
		res[j++] = s[start++];
	res[j] = 0;
	return (res);
}

static void	get_ptr(char **ptr, char const *s, char c)
{
	int index;
	int check;
	int i;

	index = 0;
	check = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			if (check == 0)
				ptr[index++] = get_ptr_child(s, c, i);
			check = 1;
		}
		else
			check = 0;
		++i;
	}
	ptr[index] = 0;
}

static void	free_ptr(char **ptr)
{
	int i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		++i;
	}
	free(ptr);
}

char		**ft_split(char const *s, char c)
{
	char	**ptr;
	int		size;
	int		i;

	if (s == NULL)
		return (NULL);
	size = get_size(s, c);
	ptr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!ptr)
		return (NULL);
	get_ptr(ptr, s, c);
	i = 0;
	while (i < size)
	{
		if (ptr[i] == NULL)
		{
			free_ptr(ptr);
			return (NULL);
		}
		++i;
	}
	return (ptr);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = (unsigned char)c;
		++i;
	}
	return (s);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, (count * size));
	return (ptr);
}

int		ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////

//utils
void	error_out(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

int	is_cub_extension(char *str)
{
	if (ft_strlen(str) < 4)
		return (0);
	while (*str)
		++str;
	str -= 4;
	if (ft_strncmp(str, ".cub", 4) == 0)
		return (1);
	return (0);	
}

char	*read_file(char *pathname)
{
	char	*res;
	char	*buf;
	int		fd;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
		error_out("Error\nFailed to open file");
	res = (char *)malloc(sizeof(char));
	buf = (char *)malloc(sizeof(char) * 2);
	if (!res || !buf)
		error_out("Error\nmalloc error");
	res[0] = 0;
	while (read(fd, buf, 1))
	{
		buf[1] = 0;
		res = ft_strjoin_free_s1(res, buf);
		if (!res)
			error_out("Error\nstrjoin error");
	}
	free(buf);
	return (res);
}

int	is_dir_flag(char c)
{
	if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (1);
	return (0);
}

char	*cut_path(char *str, char *cut)
{
	while (*cut && *str)
	{
		if (*cut != *str)
			return (NULL);
		++cut;
		++str;
	}
	while (*str == ' ')
		++str;
	return (str);
}

int	cut_rgb(char *str)
{
	int res;

	if (!ft_isdigit(*str))
		return (-1);
	res = 0;
	while (*str && ft_isdigit(*str))
	{
		res = (res * 10) + (*str - '0');
		++str;
	}
	while (*str == ' ')
		++str;
	if (*str == ',')
		++str;
	while (*str == ' ')
		++str;
	if (0 <= res && res <= 255)
		return (res);
	else
		return (-1);
}

//init
void	init_file(t_file *file)
{
	int i;

	file->str = NULL;
	file->split = NULL;
	i = 0;
	while (i < 6)
		file->info[i++] = NULL;
	file->map = NULL;
	file->map_w = 0;
	file->map_h = 0;
	file->c_flag = 0;
}

int	**init_buf(void)
{
	int	**res;
	int	*tmp;
	int	i;

	res = (int **)malloc(sizeof(int *) * HEIGHT);
	if (!res)
		error_out("Error\nmalloc error");
	i = 0;
	while (i < HEIGHT)
	{
		tmp = (int *)ft_calloc(WIDTH, sizeof(int));
		if (!tmp)
			error_out("Error\nmalloc error");
		res[i] = tmp;
		++i;
	}
	return (res);
}

void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	game->key.w = 0;
	game->key.s = 0;
	game->key.a = 0;
	game->key.d = 0;
	game->key.left = 0;
	game->key.right = 0;
	game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->img.data = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp, \
										&game->img.size_l, &game->img.endian);
	game->buf = init_buf();
	game->movespeed = 0.05;
	game->rotspeed = 0.05;
}

//check
void	check_info(t_file *file)
{
	int i;

	i = -1;
	while (++i < 6)
	{
		if (file->split[i] == NULL)
			error_out("Error\ninvalid file");
		if (ft_strncmp(file->split[i], "NO ", 3) == 0)
			file->info[NO] = file->split[i];
		else if (ft_strncmp(file->split[i], "SO ", 3) == 0)
			file->info[SO] = file->split[i];
		else if (ft_strncmp(file->split[i], "WE ", 3) == 0)
			file->info[WE] = file->split[i];
		else if (ft_strncmp(file->split[i], "EA ", 3) == 0)
			file->info[EA] = file->split[i];
		else if (ft_strncmp(file->split[i], "F ", 2) == 0)
			file->info[F] = file->split[i];
		else if (ft_strncmp(file->split[i], "C ", 2) == 0)
			file->info[C] = file->split[i];
	}
	while (--i >= 0)
	{
		if (!file->info[i])
			error_out("Error\ninvalid file");
	}
}

void	scan_map(t_file *file)
{
	int	i;
	int	j;

	i = 6;
	if (!file->split[i])
		error_out("Error\ninvalid map");
	while (file->split[i])
	{
		j = 0;
		while (file->split[i][j])
			++j;
		if (file->map_w < j)
			file->map_w = j;
		++file->map_h;
		++i;
	}
}

void	append_map(t_file *file)
{
	int	i;
	int	j;

	file->map = (char **)malloc(sizeof(char *) * (file->map_h + 1));
	i = 0;
	while (i < file->map_h)
	{
		file->map[i] = (char *)ft_calloc(file->map_w + 1, sizeof(char));
		j = 0;
		while (file->split[i + 6][j])
		{
			file->map[i][j] = file->split[i + 6][j];
			j++;
		}
		while (j < file->map_w)
			file->map[i][j++] = ' ';
		file->map[i][j] = 0;
		++i;
	}
	file->map[i] = 0;
}

void	check_surround(t_file *file, int y, int x)
{
	if ((x == 0 || x == file->map_w - 1 || y == 0 || y == file->map_h - 1) \
		&& !(file->map[y][x] == '1' || file->map[y][x] == ' '))
			error_out("Error\ninvalid map");
	if (file->map[y][x] == ' ')
	{
		if (x != 0 && !(file->map[y][x - 1] == '1' \
					|| file->map[y][x - 1] == ' '))
			error_out("Error\ninvalid map");
		if (y != 0 && !(file->map[y - 1][x] == '1' \
					|| file->map[y - 1][x] == ' '))
			error_out("Error\ninvalid map");
		if (x != file->map_w - 1 && !(file->map[y][x + 1] == '1' \
					|| file->map[y][x + 1] == ' '))
			error_out("Error\ninvalid map");
		if (y != file->map_h - 1 && !(file->map[y + 1][x] == '1' \
					|| file->map[y + 1][x] == ' '))
			error_out("Error\ninvalid map");
	}
}

void	check_map(t_file *file)
{
	int i;
	int j;

	scan_map(file);
	append_map(file);
	i = -1;
	while (file->map[++i])
	{
		j = -1;
		while (file->map[i][++j])
		{
			if (!(file->map[i][j] == '1' || file->map[i][j] == '0' || file->map[i][j] == ' '))
			{
				if (!file->c_flag && is_dir_flag(file->map[i][j]))
					file->c_flag = 1;
				else
					error_out("Error\ninvalid map");
			}
			check_surround(file, i , j);
		}
	}
	if (!file->c_flag)
		error_out("Error\ninvalid map");
}


//parse
int	*load_texture(t_game *game, char *path, t_img *img)
{
	int	*res;
	int	x;
	int	y;

	img->img = mlx_xpm_file_to_image(game->mlx, path, &img->width, &img->height);
	if (!img->img || img->width != TEX_SIZE || img->height != TEX_SIZE)
		error_out("Error\ninvalid texture");
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	res = (int *)malloc(sizeof(int) * (TEX_SIZE * TEX_SIZE));
	y = -1;
	while (++y < TEX_SIZE)
	{
		x = -1;
		while (++x < TEX_SIZE)
			res[y * TEX_SIZE + x] = img->data[y * TEX_SIZE + x];
	}
	mlx_destroy_image(game->mlx, img->img);
	return (res);
}

int	get_rgb(char *str)
{
	int r;
	int g;
	int b;

	r = cut_rgb(str);
	g = cut_rgb(str);
	b = cut_rgb(str);
	if (r == -1 || g == -1 || b == -1)
		error_out("Error\ninvalid rgb");
	return ((r * 256 * 256) + (g * 256) + b);
}

void	parse_info(t_game *game, t_file file)
{
	t_img	tmp;
	char	*path;

	path = cut_path(file.info[NO], "NO");
	game->texture[NO] = load_texture(game, path, &tmp);
	path = cut_path(file.info[SO], "SO");
	game->texture[SO] = load_texture(game, path, &tmp);
	path = cut_path(file.info[WE], "WE");
	game->texture[WE] = load_texture(game, path, &tmp);
	path = cut_path(file.info[EA], "EA");
	game->texture[EA] = load_texture(game, path, &tmp);
	path = cut_path(file.info[C], "C");
	game->ceiling = get_rgb(path);
	path = cut_path(file.info[F], "F");
	game->floor = get_rgb(path);
}

void	make_game(t_game *game, t_file file)
{
	init_game(game);
	parse_info(game, file);
	game->map = file.map;
	game->map_w = file.map_w;
	game->map_h = file.map_h;
}


//main
void	check_file(t_file *file, char *path)
{
	init_file(file);
	file->str = read_file(path);
	file->split = ft_split(file->str, '\n');
	if (!file->split)
		error_out("Error\nsplit error");
	check_info(file);
	check_map(file);
}

int main(int ac, char *av[])
{
	t_file	file;
	t_game	game;

	if (ac != 2 || !is_cub_extension(av[1]))
		error_out("Error\nusage: ./cub3d [map.cub]");
	check_file(&file, av[1]);
	make_game(&game, file);


	return (0);
}