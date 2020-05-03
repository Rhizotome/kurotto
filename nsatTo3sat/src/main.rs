use std::error;
use std::fs;
type Result<T> = std::result::Result<T, Box<dyn error::Error>>;

fn main() -> Result<()> {
    let mut nb_variables: usize = 0;
    // La ligne suivante récupère le fichier dont le nom est le premier argument passé au programme,
    // et place dans parsed la liste des clauses, et initialise nb_litteraux
    // à la valeur données par le fichier
    let parsed: Vec<Vec<isize>> = fs::read_to_string(
        std::env::args()
            .collect::<Vec<String>>()
            .get(1)
            .expect("Nombre insuffisant d'arguments."),
    )?
    .lines()
    .filter_map(|l| match l.split_whitespace().next() {
        Some("c") | None => None,
        Some("p") => {
            nb_variables = l.split_whitespace().nth(2)?.parse::<usize>().unwrap();
            None
        }
        Some(_) => Some(
            l.split_whitespace()
                .filter_map(|n| {
                    if n.parse::<isize>().unwrap() == 0 {
                        None
                    } else {
                        Some(n.parse::<isize>().unwrap())
                    }
                })
                .collect::<Vec<isize>>(),
        ),
    })
    .collect();
    // On récupère d'abord le nombre final de variables et de clauses. On conserve nb_variable afin
    // de nommer les variables supplémentaires nécessaires
    let (nb_clauses, nb_variable_final) =
        parsed
            .iter()
            .fold((0, nb_variables), |(cl, li), x| match x.len() {
                1 | 2 | 3 => (cl + 1, li),
                _ => (cl + x.len() - 2, li + x.len() - 3),
            });
    println!("p cnf {} {}", nb_variable_final, nb_clauses);
    for c in parsed.iter() {
        match c.len() {
            1 => {
                println!("{} {} {} 0", c[0], c[0], c[0]);
            }
            2 => {
                println!("{} {} {} 0", c[0], c[0], c[1]);
            }
            3 => {
                println!("{} {} {} 0", c[0], c[1], c[2]);
            }
            k => {
                nb_variables += 1;
                println!("{} {} {} 0", c[0], c[1], nb_variables);
                for i in 2..k - 2 {
                    nb_variables += 1;
                    println!("{} {} {} 0", -(nb_variables as isize) + 1, c[i], nb_variables);
                }
                println!("{} {} {} 0", -(nb_variables as isize), c[k - 2], c[k - 1]);
            }
        }
    }
    Ok(())
}
