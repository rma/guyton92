#library(rma.g92)
library(ggplot2)

#
# ggplot2 theme
#
base_size <- 16
plot.theme <- theme_grey(base_size)
plot.theme$panel.background <- theme_rect(fill = NA, colour = NA)
plot.theme$panel.grid.major <- theme_line(colour = "grey25", size = 0.50)
plot.theme$panel.grid.minor <- theme_line(colour = "grey50", size = 0.25)
plot.theme$axis.text.x <- theme_text(colour = "black", size = base_size * 0.8,
                                     vjust = 1, lineheight = 0.9)
plot.theme$axis.text.y <- theme_text(colour = "black", size = base_size * 0.8,
                                     hjust = 1, lineheight = 0.9)
plot.theme$panel.border <- theme_rect(fill = NA, colour = "black", size = 1.5)
plot.theme$strip.background <- theme_rect(colour = NA, fill = NA)
plot.theme$strip.text.x <- theme_text(colour = "black", size = base_size)
theme_set(plot.theme)

#
# simulation results
#
ut1 <- read.table("uttamsingh_1.out", header=TRUE)
bs <- read.table("data/Baldes_and_Smirk.dat", header=FALSE)
ut1.ut <- read.table("data/Uttam_Fig3_model.dat", header=FALSE)
ut2 <- read.table("uttamsingh_2.out", header=TRUE)
ut2.ut <- read.table("data/Uttam_Fig4_model.dat", header=FALSE)
ut2.dt <- read.table("data/Uttam_Fig4_data.dat", header=FALSE)
ut3 <- read.table("uttamsingh_3.out", header=TRUE)
ut3.ald.ut <- read.table("data/Uttam_Fig5_ald_model.dat", header=FALSE)
ut3.ald.dt <- read.table("data/Uttam_Fig5_ald_data.dat", header=FALSE)
ut3.ecf.ut <- read.table("data/Uttam_Fig5_ecf_model.dat", header=FALSE)
ut3.ecf.dt1 <- read.table("data/Uttam_Fig5_ecf_data1.dat", header=FALSE)
ut3.ecf.dt2 <- read.table("data/Uttam_Fig5_ecf_data2.dat", header=FALSE)
ut3.map.ut <- read.table("data/Uttam_Fig5_map_model.dat", header=FALSE)
ut3.map.dt <- read.table("data/Uttam_Fig5_map_data.dat", header=FALSE)
ut3.na.ut <- read.table("data/Uttam_Fig5_na_model.dat", header=FALSE)
ut3.na.dt1 <- read.table("data/Uttam_Fig5_na_data1.dat", header=FALSE)
ut3.na.dt2 <- read.table("data/Uttam_Fig5_na_data2.dat", header=FALSE)
ik7o <- read.table("ikeda_7.out", header=TRUE)
ik7a <- read.table("ikeda_7a.out", header=TRUE)

#
# time scaling
#
ut1$t <- (ut1$t - 40320) / 60
ut2$t <- (ut2$t - 40320) / 60
ut3$t <- (ut3$t - 40320) / (60 * 24) - (30/60)
ik7o$t <- (ik7o$t - 40320) / (60 * 24)
ik7a$t <- (ik7a$t - 40320) / (60 * 24)

ik7 <- rbind(ik7o, ik7a)
ik7$exp <- c(rep("Full", 74), rep("Maint", 74))

#
# plotting
#
ik7plot <- function(var.name) {
    qplot(x = ik7$t, y = ik7[[var.name]], colour = ik7$exp) +
        xlab("Time (days)") +
        ylab(toupper(var.name)) +
        scale_colour_hue("Treatment")
}

cairo_pdf(filename = "uttamsingh_1.pdf", onefile = TRUE,
          width = 7, height = 7)
ut1.xs <- c(ut1$t, bs$V1, ut1.ut$V1)
ut1.ys <- c(ut1$vud * 1e3, bs$V2, ut1.ut$V2)
ut1.lbl <- c(rep('Guyton', length(ut1$t)),
             rep('Baldes & Smirk', length(bs$V1)),
             rep('Uttamsingh', length(ut1.ut$V1)))
#qplot(x = ut1.xs, y = ut1.ys, colour=ut1.lbl) +
qplot() +
    geom_line(aes(ut1$t, ut1$vud * 1e3, colour="Guyton"), size=1) +
    geom_line(aes(ut1.ut$V1, ut1.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(bs$V1, bs$V2, shape="Baldes & Smirk"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlab("Time (hours)") +
    ylab("Urine flow rate (mL/min)") +
    opts(legend.position=c(0.8,0.85),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_2.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(ut2$t, ut2$vud * 1e3, colour="Guyton"), size=1) +
    geom_line(aes(ut2.ut$V1, ut2.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(ut2.dt$V1, ut2.dt$V2, shape="Dean & McCance"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlab("Time (hours)") +
    ylab("Urine flow rate (mL/min)") +
    xlim(0, 8) +
    opts(legend.position=c(0.8,0.85),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_2_norm.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(ut2$t, ut2$vud * 1e3 * 2, colour="Guyton"), size=1) +
    geom_line(aes(ut2.ut$V1, ut2.ut$V2 / 2, colour="Uttamsingh"), size=1) +
    geom_point(aes(ut2.dt$V1, ut2.dt$V2 / 2, shape="Dean & McCance"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlab("Time (hours)") +
    ylab("Urine flow rate (normalised)") +
    xlim(0, 8) +
    opts(legend.position=c(0.8,0.85),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_ecf.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(x = ut3$t, y = ut3$vec, colour="Guyton"), size=1) +
    geom_line(aes(x = ut3.ecf.ut$V1, y = ut3.ecf.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(x = ut3.ecf.dt1$V1, y = ut3.ecf.dt1$V2, shape="Davis & Howell"), size=3) +
    geom_point(aes(x = ut3.ecf.dt2$V1, y = ut3.ecf.dt2$V2, shape="Relman & Schwartz"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("ECF (L)") +
    opts(legend.position=c(0.8,0.35),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_map.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(x = ut3$t + 0.25, y = ut3$pa, colour="Guyton"), size=1) +
    geom_line(aes(x = ut3.map.ut$V1, y = ut3.map.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(x = ut3.map.dt$V1, y = ut3.map.dt$V2, shape="Davis & Howell"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("MAP (mmHg)") +
    opts(legend.position=c(0.8,0.25),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_ald.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(x = ut3$t, y = ut3$amc, colour="Guyton"), size=1) +
    geom_line(aes(x = ut3.ald.ut$V1, y = ut3.ald.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(x = ut3.ald.dt$V1, y = ut3.ald.dt$V2, shape="Relman & Schwartz"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("Aldosterone (normalised)") +
    opts(legend.position=c(0.8,0.25),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_vud.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot(x = ut3$t, y = ut3$vud * 1e3) +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("Urine flow rate (mL/min)") +
    opts(legend.position=c(0.8,0.25),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_nod.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(x = ut3$t, y = ut3$nod, colour="Guyton"), size=1) +
    geom_line(aes(x = ut3.na.ut$V1, y = ut3.na.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(x = ut3.na.dt1$V1, y = ut3.na.dt1$V2, shape="Davis & Howell"), size=3) +
    geom_point(aes(x = ut3.na.dt2$V1, y = ut3.na.dt2$V2, shape="Relman & Schwartz"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("Sodium excretion (mEq/min)") +
    opts(legend.position=c(0.8,0.25),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "uttamsingh_3_nax.pdf", onefile = TRUE,
          width = 7, height = 7)
qplot() +
    geom_line(aes(x = ut3$t, y = 1e-3 * ut3$nod / ut3$vud, colour="Guyton"), size=1) +
    geom_line(aes(x = ut3.na.ut$V1, y = ut3.na.ut$V2, colour="Uttamsingh"), size=1) +
    geom_point(aes(x = ut3.na.dt1$V1, y = ut3.na.dt1$V2, shape="Davis & Howell"), size=3) +
    geom_point(aes(x = ut3.na.dt2$V1, y = ut3.na.dt2$V2, shape="Relman & Schwartz"), size=3) +
    scale_colour_hue("Model") +
    scale_shape("Data") +
    xlim(0, 7) +
    xlab("Time (days)") +
    ylab("Sodium excretion (mEq/L)") +
    opts(legend.position=c(0.8,0.25),
         legend.background = theme_rect(colour = 'black', fill = 'white', size = 0.5))
dev.off()

cairo_pdf(filename = "ikeda_7_ecf.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vec"))
dev.off()
cairo_pdf(filename = "ikeda_7_isf.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vif"))
dev.off()
cairo_pdf(filename = "ikeda_7_vtw.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vtw"))
dev.off()
cairo_pdf(filename = "ikeda_7_vud.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vud"))
dev.off()
cairo_pdf(filename = "ikeda_7_icf.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vic"))
dev.off()
cairo_pdf(filename = "ikeda_7_vp.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("vp"))
dev.off()
cairo_pdf(filename = "ikeda_7_cna.pdf", onefile = TRUE,
          width = 7, height = 7)
print(ik7plot("cna"))
dev.off()
