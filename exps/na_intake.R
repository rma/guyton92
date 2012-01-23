#!/usr/bin/r --vanilla

library(ggplot2)

# Read the output from each simulation, over a range of NID (sodium intake).
nid.frame <- function() {
    input.data = list()

    nid.seqn <- seq(0.02, 0.40, 0.02)
    nid.names <- sprintf('%0.2f', nid.seqn)

    for (nid in nid.names) {
        infile <- paste('na_intake_', nid, '.out', sep="")
        input.data[[nid]] <- read.table(infile, header=TRUE)
    }

    # Get the names of the outputs, ignoring time ('t').
    output.vars <- names(input.data[["0.02"]])[-1]

    row.count <- dim(input.data[["0.02"]])[1]
    row.ixs <- seq(row.count / 2, row.count)

    input.means = list()

    output.frame <- data.frame(nid = nid.seqn)

    for (outvar in output.vars) {
        var.means <- c()
        for (tbl.name in names(input.data)) {
            tbl.mean <- mean(input.data[[tbl.name]][[outvar]][row.ixs])
            var.means <- c(var.means, tbl.mean)
          }
        output.frame[[outvar]] <- var.means
      }

    output.frame$nosm <- output.frame$nod / output.frame$vud
    return(output.frame)
}

# Set the ggplot2 theme to one more suited for printing.
set.ggplot2.theme <- function (base.size=16) {
    plot.theme <- theme_get()
    plot.theme$panel.background <- theme_rect(fill = NA, colour = NA)
    plot.theme$panel.grid.major <- theme_line(colour = "grey25", size = 0.50)
    plot.theme$panel.grid.minor <- theme_line(colour = "grey50", size = 0.25)
    plot.theme$axis.text.x <- theme_text(colour = "black",
                                         size = base.size * 0.8,
                                         vjust = 1, lineheight = 0.9)
    plot.theme$axis.text.y <- theme_text(colour = "black",
                                         size = base.size * 0.8,
                                         hjust = 1, lineheight = 0.9)
    plot.theme$axis.title.x <- theme_text(colour = "black",
                                          size = base.size,
                                          vjust = 1, lineheight = 0.9)
    plot.theme$axis.title.y <- theme_text(colour = "black",
                                          size = base.size,
                                          hjust = 1, lineheight = 0.9,
                                          angle = 90)
    plot.theme$panel.border <- theme_rect(fill = NA, colour = "black",
                                          size = 1.5)
    plot.theme$strip.background <- theme_rect(colour = NA, fill = NA)
    plot.theme$strip.text.x <- theme_text(colour = "black", size = base.size)
    theme_set(plot.theme)
}

# The axis label for each output variable.
ylabels <- list(
    adhc   = "ADH concentration (normalised)",
    adhmk  = "ADH effect on Na and H2O reabsorption",
    am     = "Aldosterone multiplier",
    amc    = "Aldosterone concentration (normalised)",
    amna   = "Aldosterone effect on Na reabsorption",
    anm    = "Angiotensin multiplier",
    cna    = "Extracellular sodium concentration (mEq/L)",
    cpp    = "Plasma protein concentration (g/L)",
    dtnang = "Distal Na reabsorption due to angiotensin (mEq/min)",
    dtnara = "Distal Na reabsorption (non-angiotensin) (mEq/min)",
    gfr    = "Glomerular filtration rate (L/min)",
    glp    = "Glomerular (hydrostatic) pressure (mmHg)",
    glpc   = "Glomerular colloid oncotic pressure (mmHg)",
    mdflw  = "Macula densa flow (normalised)",
    osmopn = "Urinary osmolar excretion (mEq/min)",
    nod    = "Sodium excretion (mEq/min)",
    nosm   = "Sodium excretion (mEq/L)",
    pa     = "Mean arterial pressure (mmHg)",
    par    = "Renal arterial pressure (mmHg)",
    pfl    = "Renal filtration pressure (mmHg)",
    ppc    = "Plasma colloid osmotic pressure (mmHg)",
    rbf    = "Renal blood flow (L/min)",
    rr     = "Total renal resistance (mmHg min/L)",
    vud    = "Rate of urine production (L/min)"
)

pd <- nid.frame()
set.ggplot2.theme()

# Produce a plot of one of the variables against NID (sodium intake).
make.plot <- function(yvar) {
    p <- qplot(pd$nid, pd[[yvar]],
               xlab="Na intake (mEq/min)",
               ylab=ylabels[[yvar]]) +
         geom_line(size=2) +
         geom_point(size=5)
    return(p)
}

# Plot each output variable and save the plots as a single PDF.
cairo_pdf(filename="na_intake.pdf", width=7, height=7, onefile=TRUE,
          bg="transparent", antialias="subpixel")

for (yvar in names(ylabels)) {
    print(make.plot(yvar))
}

dev.off()
