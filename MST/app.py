from graph import enter, find_spanning_tree


def main() -> None:
    """
    Program entrypoint
    """

    mst = []

    selected_menu_item_number = -1
    while selected_menu_item_number != 3:
        print("1 - Ввести граф")
        print("2 - Показать остов")
        print("3 - Выйти")

        selected_menu_item_number = input("< ")

        try:
            if selected_menu_item_number.isdigit():
                selected_menu_item_number = int(selected_menu_item_number)
                if 0 <= selected_menu_item_number <= 3:
                    if selected_menu_item_number == 1:
                        mst = find_spanning_tree(enter)
                    if selected_menu_item_number == 2:
                        print("Остов: ", mst)
                        mst = []
                        continue
                    if selected_menu_item_number == 3:
                        break
                else:
                    print("Неправильно введен номер пункта меню")
                    continue
            else:
                print("Неправильно введен номер пункта меню")
                continue
        except ValueError:
            print("Что-то пошло не так")


if __name__ == '__main__':
    main()
