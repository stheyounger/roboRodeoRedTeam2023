import v1.RandomMath
import java.math.BigDecimal
import kotlin.math.*

data class Point2D(val x: Double, val y: Double) {
    operator fun plus(other: Any): Point2D {
        other as Point2D
        return Point2D(this.x + other.x, this.y + other.y)
    }
    operator fun minus(other: Any): Point2D {
        other as Point2D
        return Point2D(this.x - other.x, this.y - other.y)
    }
    fun angleToRadians(other: Point2D): Double {
        return atan2(other.y - this.y, other.x - this.x)
    }

    fun distanceTo(other: Point2D): Double {
        val distance = sqrt((other.y - this.y).pow(2) + (other.x - this.x).pow(2))
        return RandomMath.roundPreciseNumber(distance)
    }

    companion object {
        private fun pointFromVector(magnitude: BigDecimal, directionRadians: BigDecimal): Point2D {
            return Point2D((magnitude * cos(directionRadians.toDouble()).toBigDecimal()).toDouble(), (magnitude * sin(directionRadians.toDouble()).toBigDecimal()).toDouble())
        }
        fun pointFromVector(magnitude: Double, directionRadians: Double): Point2D {
            return pointFromVector(magnitude.toBigDecimal(), directionRadians.toBigDecimal())
        }
    }

    override fun toString(): String = "v1.Point2D($x, $y)"
}