/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taejkim <taejkim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:20:42 by taejkim           #+#    #+#             */
/*   Updated: 2022/04/21 18:38:10 by taejkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*cut_path(char *str, char *cut)
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

static int	*load_texture(t_game *game, char *path, t_img *img)
{
	int	*res;
	int	x;
	int	y;

	img->img = mlx_xpm_file_to_image(game->mlx,
			path, &img->width, &img->height);
	if (!img->img || img->width != TEX_SIZE || img->height != TEX_SIZE)
		error_out("Error\ninvalid texture");
	img->data = (int *)mlx_get_data_addr(img->img,
			&img->bpp, &img->size_l, &img->endian);
	res = (int *)malloc(sizeof(int) * (TEX_SIZE * TEX_SIZE));
	if (!res)
		error_out("Error\nmalloc error");
	y = -1;
	while (++y < TEX_SIZE)
	{
		x = -1;
		while (++x < TEX_SIZE)
			res[y * TEX_SIZE + x] = img->data[x * TEX_SIZE + y];
	}
	mlx_destroy_image(game->mlx, img->img);
	return (res);
}

static void	parse_info(t_game *game, t_file file)
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
	game->pos_x = file.c_x + 0.5;
	game->pos_y = file.c_y + 0.5;
	if (file.c_flag == 'N')
		set_dir_plane_n(game);
	if (file.c_flag == 'S')
		set_dir_plane_s(game);
	if (file.c_flag == 'W')
		set_dir_plane_w(game);
	if (file.c_flag == 'E')
		set_dir_plane_e(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	mlx_mouse_hide();
}
