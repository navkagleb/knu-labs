package spring.persistence.entity;

import lombok.Data;
import lombok.experimental.Accessors;

import javax.persistence.Embeddable;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import java.io.Serializable;

@Embeddable
@Data
@Accessors(chain = true)
public class OrderDishPK implements Serializable {

    private static final long serialVersionUID = 1876011689392953634L;

    @ManyToOne(optional = false, fetch = FetchType.LAZY)
    @JoinColumn(name = "order_id")
    private Order order;

    @ManyToOne(optional = false, fetch = FetchType.LAZY)
    @JoinColumn(name = "dish_id")
    private Dish dish;
}
